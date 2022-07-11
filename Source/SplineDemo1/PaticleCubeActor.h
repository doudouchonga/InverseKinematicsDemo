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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InitPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector OldPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CurPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed{0.1f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Percent{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector VeletDeltaPos;
	

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
		void VeletPercent(float percent);

	UFUNCTION(BlueprintCallable)
		void VeletDelta(float distance, float delta);

	UFUNCTION(BlueprintCallable)
	void SolveDistance();

	UFUNCTION(BlueprintCallable)
		void CreateParticle(FVector pos, bool free);

	UFUNCTION(BlueprintCallable)
		void SetParticleTargetPos(int x, FVector pos, float time = 3.0f);

	UFUNCTION(BlueprintCallable)
		void SetParticleProperty(int x, FVector pos, bool free);

	UFUNCTION(BlueprintCallable)
		void ResetParticlePercent();

	UFUNCTION(BlueprintCallable)
	void BuildConstrain(int x, int y);

	UFUNCTION(BlueprintCallable)
		void ResetParticles();

	void SolveDistance(FVConstrain& Constrain);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DisierDistance{10};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTickVelet{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<APaticleCubeActor*> InTargets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int InTargetIndex{1};
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
