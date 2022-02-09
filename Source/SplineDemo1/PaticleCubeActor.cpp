// Fill out your copyright notice in the Description page of Project Settings.


#include "PaticleCubeActor.h"
#include "Engine/StaticMeshActor.h"

#include "Materials/Material.h"
#include "../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "../Plugins/Experimental/GeometryProcessing/Source/GeometricObjects/Public/MathUtil.h"

// Sets default values
APaticleCubeActor::APaticleCubeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("mesh"));
	MeshMaterial = LoadObject<UMaterial>(NULL, TEXT("/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Worn"), NULL, LOAD_None, NULL);
	
}


void APaticleCubeActor::Velet()
{

	for (int32 i = 0; i < Particles.Num(); i++)
	{
		if (!Particles[i].bFree)
		{
			continue;
		}
		// Find vel
		const FVector Vel = Particles[i].CurPos - Particles[i].OldPos;
		FVector TargetPosition = Particles[i].TargetPos - Particles[i].CurPos;
		TargetPosition.Normalize();
		// Update position
		const FVector NewPosition = Particles[i].CurPos + Vel + (Particles[i].Speed * TargetPosition);
		Particles[i].OldPos = Particles[i].CurPos;
		Particles[i].CurPos = NewPosition;
	}
}

void APaticleCubeActor::SolveDistance()
{
	for (FVConstrain& item : Constrains)
	{
		SolveDistance(item);
	}

}

void APaticleCubeActor::SolveDistance(FVConstrain& Constrain)
{
	FVParticle* particleA = &Particles[Constrain.iparticleA];
	FVParticle* particleB = &Particles[Constrain.iparticleB];

	if (particleA == nullptr || particleB == nullptr) return;
	
	FVector Delta = particleB->CurPos - particleA->CurPos;

	float CurrentDistance = Delta.Size();
	float ErrorFactor = (CurrentDistance - DisierDistance) / CurrentDistance;

	if (particleA->bFree && particleB->bFree)
	{
		particleA->CurPos += ErrorFactor * 0.5f * Delta;
		particleB->CurPos -= ErrorFactor * 0.5f * Delta;
	}
	else if (particleA->bFree)
	{
		particleA->CurPos += ErrorFactor * Delta;
	}
	else if (particleB->bFree)
	{
		particleB->CurPos -= ErrorFactor * Delta;
	}
}

void APaticleCubeActor::CreateParticle(FVector pos, bool free)
{
	FVParticle item;
	item.CurPos = pos;
	item.OldPos = pos;
	item.bFree = free;
	Particles.Add(item);
}

void APaticleCubeActor::SetParticleTargetPos(int x, FVector pos, float time)
{
	if (x < 0 || x >= Particles.Num())
	{
		return;
	}
	FVParticle& particleA = Particles[x];
	particleA.SetTargetPos(pos);

	particleA.Speed = FVector::Distance(particleA.TargetPos, particleA.CurPos) / time;

}

void APaticleCubeActor::BuildConstrain(int x, int y)
{
	if (x < 0 || x >= Particles.Num())
	{
		return;
	}
	if (y < 0 || y >= Particles.Num())
	{
		return;
	}
	FVConstrain item;
	item.iparticleA = x;
	item.iparticleB = y;
	Constrains.Add(item);

}


// Called when the game starts or when spawned
void APaticleCubeActor::BeginPlay()
{
	Super::BeginPlay();
		
	/*PaticleCubeActor =  GetWorld()->SpawnActor<AUnitPaticleCubeActor>(AUnitPaticleCubeActor::StaticClass());

	PaticleCubeActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);*/
}

// Called every frame
void APaticleCubeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Velet();
	SolveDistance();

}
