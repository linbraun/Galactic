// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "FogActor.generated.h"

UCLASS()
class PAINTBALL_STOUGHTON_API AFogActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFogActor();

	/*
	Added for fog generation - LS 1/30/23
	*/
	virtual void PostInitializeComponents() override;

	// Set the fog size
	void setSize(float s);

	// Reveal a portion of the fog
	void revealSmoothCircle(const FVector2D & pos, float radius);

private:
	void UpdateTextureRegions(UTexture2D * Texture, int32 MipIndex, uint32 NumRegions,
		FUpdateTextureRegion2D * Regions, uint32 SrcPitch, uint32 SrcBpp, uint8 * SrcData, bool bFreeData);

	// Fog texture size
	static const int m_textureSize = 512;
	
	UPROPERTY() UStaticMeshComponent * m_squarePlane;
	UPROPERTY() UTexture2D * m_dynamicTexture;
	UPROPERTY() UMaterialInterface * m_dynamicMaterial;
	UPROPERTY() UMaterialInstanceDynamic * m_dynamicMaterialInstance;
	uint8 m_pixelArray[m_textureSize * m_textureSize];
	FUpdateTextureRegion2D m_wholeTextureRegion;
	float m_coverSize;

	/**/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
