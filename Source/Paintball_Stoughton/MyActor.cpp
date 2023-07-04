// Fill out your copyright notice in the Description page of Project Settings.

#include "MyActor.h"


// Sets default values
// Added procedural mesh generation to constructor - LS 1/23/23
AMyActor::AMyActor() {
	mesh = CreateDefaultSubobject <UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*
Added the following for procedural mesh generation - LS 1/23/23
*/
void AMyActor::PostActorCreated() {
	Super::PostActorCreated();
	CreateSquare();
} // This is called when actor is already in level and map is opened.
void AMyActor::PostLoad() {
	Super::PostLoad();
	CreateSquare();
}
void AMyActor::CreateSquare() {
	TArray <FVector> Vertices;
	TArray <int32> Triangles;
	TArray <FVector> Normals;
	TArray < FLinearColor > Colors;
	Vertices.Add(FVector(0.f, 0.f, 0.f));
	Vertices.Add(FVector(0.f, 100.f, 0.f));
	Vertices.Add(FVector(0.f, 0.f, 100.f));
	Vertices.Add(FVector(0.f, 100.f, 100.f));
	Triangles.Add(0);
	Triangles.Add(1);
	Triangles.Add(2);
	Triangles.Add(3);
	Triangles.Add(2);
	Triangles.Add(1);
	for (int32 i = 0; i < Vertices.Num(); i++) {
		Normals.Add(FVector(0.f, 0.f, 1.f));
		Colors.Add(FLinearColor::Red);
	} // Optional arrays.
	TArray <FVector2D> UV0;
	TArray <FProcMeshTangent> Tangents;
	mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, Colors, Tangents,
		true);
}