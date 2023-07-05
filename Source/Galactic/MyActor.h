// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h" // Included procedural mesh generation - LS 1/23/23
#include "MyActor.generated.h"


UCLASS()
class GALACTIC_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
/*
Added the following for procedural mesh generation - LS 1/23/23
*/
public:
	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
	virtual void CreateSquare();
private:
	UPROPERTY(VisibleAnywhere)
		UProceduralMeshComponent * mesh;
};
