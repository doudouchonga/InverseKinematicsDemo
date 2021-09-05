// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LineActor.generated.h"

UCLASS()
class SPLINEDEMO1_API ALineActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALineActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProceduralMeshComponent* ProceduralMesh;
	UPROPERTY()
		class UMaterial* MeshMaterial;

	int m_nIndex;
	
	UFUNCTION(BlueprintCallable)
	void Update2DOFKnee(TArray<FVector> nodes, FVector targetPos, FVector direction, TArray<FVector>& OutNodes);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
