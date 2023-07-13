// Fill out your copyright notice in the Description page of Project Settings.

#include "FogActor.h"
#include "Engine.h"

// Sets default values
AFogActor::AFogActor():m_wholeTextureRegion(0, 0, 0, 0, m_textureSize, m_textureSize) {
	m_coverSize = 5000;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/* 
	Added for fog generation - LS 1/30/23 
	*/

	// Create a planar mesh from engine's planar static mesh.
	m_squarePlane = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Fog Plane Static Mesh"));
	RootComponent = m_squarePlane;
	m_squarePlane->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName); {
		static ConstructorHelpers::FObjectFinder <UStaticMesh>
	asset(TEXT("/Engine/ArtTools/RenderToTexture/Meshes/S_1_Unit_Plane.S_1_Unit_Plane"));
		m_squarePlane->SetStaticMesh(asset.Object);
	}
	m_squarePlane->TranslucencySortPriority = 100;
	m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));
	// Load the base material from your created material.
	{
		static ConstructorHelpers::FObjectFinder <UMaterial>
	asset(TEXT("Material'/Game/Shaders/FogMat.FogMat'"));
		m_dynamicMaterial = asset.Object;
	}
	// Create the run-time fog texture.
	if (!m_dynamicTexture) {
		m_dynamicTexture = UTexture2D::CreateTransient(m_textureSize, m_textureSize, PF_G8);
		m_dynamicTexture->CompressionSettings = TextureCompressionSettings::TC_Grayscale;
		m_dynamicTexture->SRGB = 0;
		m_dynamicTexture->UpdateResource();
	
		// Added endif to suppress warning when packaging - LB 7/6/23
		#if WITH_EDITORONLY_DATA
		m_dynamicTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		#endif
	}
	// Initialize array to all black.
	for (int x = 0; x < m_textureSize; ++x)
		for (int y = 0; y < m_textureSize; ++y) m_pixelArray[y * m_textureSize + x] = 255;
	// Propagate memory's array to texture.
	if (m_dynamicTexture) UpdateTextureRegions(m_dynamicTexture, 0, 1, &m_wholeTextureRegion,
	m_textureSize, 1, m_pixelArray, false);
}

void AFogActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime); // Call parent class tick function.
}

void AFogActor::PostInitializeComponents() {
	Super::PostInitializeComponents();
	// Create a dynamic material instance to swap in the fog texture.
	if (m_dynamicMaterial) {
		m_dynamicMaterialInstance = UMaterialInstanceDynamic::Create(m_dynamicMaterial, this);
		m_dynamicMaterialInstance->SetTextureParameterValue("FogTexture", m_dynamicTexture);
	}
	// Set the dynamic material to the mesh.
	if (m_dynamicMaterialInstance) m_squarePlane->SetMaterial(0, m_dynamicMaterialInstance);
}

void AFogActor::setSize(float s) {
	m_coverSize = s;
	m_squarePlane->SetRelativeScale3D(FVector(m_coverSize, m_coverSize, 1));
}

void AFogActor::revealSmoothCircle(const FVector2D & pos, float radius) { // Calculate the where circle center is inside texture space
	FVector2D texel = pos - FVector2D(GetActorLocation().X, GetActorLocation().Y);
	texel = texel * m_textureSize / m_coverSize;
	texel += FVector2D(m_textureSize / 2, m_textureSize / 2); // Calculate radius in texel unit ( 1 is 1 pixel)
	float texelRadius = radius * m_textureSize / m_coverSize; // The square area to update
	int minX = FMath::Clamp <int>(texel.X - texelRadius, 0, m_textureSize - 1);
	int minY = FMath::Clamp <int>(texel.Y - texelRadius, 0, m_textureSize - 1);
	int maxX = FMath::Clamp <int>(texel.X + texelRadius, 0, m_textureSize - 1);
	int maxY = FMath::Clamp <int>(texel.Y + texelRadius, 0, m_textureSize - 1);
	uint8 theVal = 0; // Update our array of fog value in memory
	bool dirty = false;
	for (int x = minX; x < maxX; ++x) {
		for (int y = minY; y < maxY; ++y) {
			float distance = FVector2D::Distance(texel, FVector2D(x, y));
			if (distance < texelRadius) {
				static float smoothPct = 0.7f;
				uint8 oldVal = m_pixelArray[y * m_textureSize + x];
				float lerp = FMath::GetMappedRangeValueClamped(FVector2D(smoothPct, 1.0f),
					FVector2D(0, 1), distance / texelRadius);
				uint8 newVal = lerp * 255;
				newVal = FMath::Min(newVal, oldVal);
				m_pixelArray[y * m_textureSize + x] = newVal;
				dirty = dirty || oldVal != newVal;
			}
		}
	} // Propagate the values in memory's array to texture.
	if (dirty) UpdateTextureRegions(m_dynamicTexture, 0, 1, &m_wholeTextureRegion, m_textureSize,
1, m_pixelArray, false);
}

void AFogActor::UpdateTextureRegions(UTexture2D * Texture, int32 MipIndex, uint32 NumRegions,
	FUpdateTextureRegion2D * Regions, uint32 SrcPitch, uint32 SrcBpp, uint8 * SrcData, bool bFreeData) {
	if (Texture->Resource) {
		struct FUpdateTextureRegionsData {
			FTexture2DResource * Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D * Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8 * SrcData;
		};
		FUpdateTextureRegionsData * RegionData = new FUpdateTextureRegionsData;
		RegionData->Texture2DResource = (FTexture2DResource *)Texture->Resource;
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		// Replaced outdated macro - LB 7/6/23
		ENQUEUE_RENDER_COMMAND(UpdateTextureRegionsData)([RegionData, bFreeData](
			FRHICommandListImmediate& RHICmdList)
			{
			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
			{
			int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
			if (RegionData->MipIndex >= CurrentFirstMip)
			{
			RHIUpdateTexture2D(
				RegionData->Texture2DResource->GetTexture2DRHI(),
				RegionData->MipIndex - CurrentFirstMip,
				RegionData->Regions[RegionIndex],
				RegionData->SrcPitch,
				RegionData->SrcData
				+ RegionData->Regions[RegionIndex].SrcY * RegionData->SrcPitch
				+ RegionData->Regions[RegionIndex].SrcX * RegionData->SrcBpp
			);
			}
			}
			if (bFreeData)
			{
			FMemory::Free(RegionData->Regions);
			FMemory::Free(RegionData->SrcData);
			}
			delete RegionData;
			});
/*
		ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(UpdateTextureRegionsData,
FUpdateTextureRegionsData *, RegionData, RegionData, bool, bFreeData, bFreeData, {
			for (uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex) {
				int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();
				if (RegionData->MipIndex >= CurrentFirstMip) {
					RHIUpdateTexture2D(RegionData->Texture2DResource->GetTexture2DRHI(), RegionData -> MipIndex - CurrentFirstMip, 
						RegionData->Regions[RegionIndex], RegionData->SrcPitch, RegionData -> SrcData + RegionData->Regions[RegionIndex].SrcY 
						* RegionData->SrcPitch + RegionData -> Regions[RegionIndex].SrcX * RegionData->SrcBpp);
				}
			}
			if (bFreeData) {
				FMemory::Free(RegionData->Regions);
				FMemory::Free(RegionData->SrcData);
			}
			delete RegionData;
		});
*/
	}
}