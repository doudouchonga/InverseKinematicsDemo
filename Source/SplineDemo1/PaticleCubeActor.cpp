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



void APaticleCubeActor::VeletPercent(float percent)
{
	if (InTargets.Num() <= 0)
	{
		return;
	}
	if (InTargetIndex <= 0)
	{
		return;
	}

	for (int32 i = 0; i < Particles.Num(); i++)
	{
		if (!Particles[i].bFree)
		{
			continue;
		}

		APaticleCubeActor* T1 = InTargets[InTargetIndex - 1];
		APaticleCubeActor* T2 = InTargets[InTargetIndex];

		FVector P2 = T2->Particles[i].CurPos - T2->Particles[0].CurPos;
		FVector P1 = T1->Particles[i].CurPos - T1->Particles[0].CurPos;
		FVector TargetPosition = P2 - P1;
		Particles[i].Percent += percent;
		Particles[i].Percent = FMathf::Min(1.0f, Particles[i].Percent);

		float TargetDistance = FVector::Distance(P1, P2);
		TargetPosition.Normalize();
		// Update position
		const FVector NewPosition = P1 + TargetDistance * Particles[i].Percent * TargetPosition + Particles[0].CurPos;
		Particles[i].OldPos = Particles[i].CurPos;
		Particles[i].CurPos = NewPosition;
	}
}

void APaticleCubeActor::VeletDelta(float distance, float delta)
{
	for (int32 i = 0; i < Particles.Num(); i++)
	{
		if (i == 0)
		{
			continue;
		}
		if (!Particles[i].bFree)
		{
			continue;
		}
		// Find vel
		const FVector Vel = Particles[i].CurPos - Particles[i].OldPos;
		FVector TargetPosition = Particles[i].TargetPos - Particles[i].CurPos;
		TargetPosition.Normalize();

		float TargetDis = FVector::Distance(Particles[i].TargetPos, Particles[i].CurPos);
		float LastDistance = FVector::Distance(Particles[i - 1].TargetPos, Particles[i - 1].CurPos);
		float Speed = Particles[i].Speed;
		if (LastDistance > 0 && TargetDis > 0)
		{
			Speed = Particles[i].Speed * TargetDis / LastDistance;
		}
		
		// Update position
		FVector NewPosition = Particles[i].CurPos + Vel + (Speed * TargetPosition) * delta;

		float MoveDis = FVector::Distance(NewPosition, Particles[i].CurPos);

		if (MoveDis >= TargetDis)
		{
			NewPosition = Particles[i].TargetPos;
		}

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
	particleA.InitPos = particleA.CurPos;
	// particleA.Speed = FVector::Distance(particleA.TargetPos, particleA.CurPos) / time;
	particleA.Speed = time;
}

void APaticleCubeActor::SetParticleProperty(int x, FVector pos, bool free)
{
	if (x < 0 || x >= Particles.Num())
	{
		return;
	}

	FVParticle item;
	item.CurPos = pos;
	item.OldPos = pos;
	item.bFree = free;
	Particles[x] = item;
}

void APaticleCubeActor::ResetParticlePercent()
{
	for (FVParticle& elm : Particles)
	{
		elm.Percent = 0;
	}
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

	if (bTickVelet)
	{
		Velet();
		SolveDistance();
	}
	

}
