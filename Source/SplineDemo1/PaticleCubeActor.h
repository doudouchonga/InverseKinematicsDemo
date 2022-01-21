// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaticleCubeActor.generated.h"

USTRUCT(BlueprintType)
struct FVParticle
{
	GENERATED_USTRUCT_BODY()
	FVParticle() :
		bFree(true),
		OldPos(0, 0, 0),
		CurPos(0, 0, 0)
	{}

	bool bFree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OldPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CurPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed{0.1f};

	void SetTargetPos(FVector pos)
	{
		TargetPos = pos;
	}

};

USTRUCT(BlueprintType)
struct FVConstrain
{
	GENERATED_USTRUCT_BODY()
	FVConstrain() {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int iparticleA;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int iparticleB;
	
	void BuildConstrain(int A, int B)
	{
		iparticleA = A;
		iparticleB = B;
	}

	
	void DrawDebugConstrain(UWorld* world)
	{
		// DrawDebugCylinder(world, particleA->CurPos, particleB->CurPos, 1.0, 6, FColor::Blue, false, -1, 0, 1.0);
	}

};

UCLASS(BlueprintType)
class SPLINEDEMO1_API APaticleCubeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaticleCubeActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY()
		class UMaterial* MeshMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVParticle> Particles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FVConstrain> Constrains;
	
	UFUNCTION(BlueprintCallable)
	void Velet();
	UFUNCTION(BlueprintCallable)
	void SolveDistance();

	UFUNCTION(BlueprintCallable)
		void CreateParticle(FVector pos, bool free);

	UFUNCTION(BlueprintCallable)
		void SetParticleTargetPos(int x, FVector pos);

	UFUNCTION(BlueprintCallable)
	void BuildConstrain(int x, int y);

	void SolveDistance(FVConstrain& Constrain);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DisierDistance{10};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
