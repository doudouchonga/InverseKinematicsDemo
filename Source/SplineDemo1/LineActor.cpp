// Fill out your copyright notice in the Description page of Project Settings.


#include "LineActor.h"
#include "Engine/StaticMeshActor.h"
#include "UnitLineActor.h"
#include "Materials/Material.h"
#include "../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "../Plugins/Experimental/GeometryProcessing/Source/GeometricObjects/Public/MathUtil.h"

// Sets default values
ALineActor::ALineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("mesh"));
	MeshMaterial = LoadObject<UMaterial>(NULL, TEXT("/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Worn"), NULL, LOAD_None, NULL);
	
}


void ALineActor::Update2DOFKnee(TArray<FVector> nodes, FVector targetPos, FVector direction, TArray<FVector>& outNodes)
{
	if (nodes.Num() < 2)
	{
		return;
	}

	FVector start = FVector(0);//GetActorLocation(); 
	FVector end = targetPos;

	float l0 = (nodes[0] - start).Size();
	float l1 = (nodes[1] - nodes[0]).Size();

	float l = (end - start).Size();
	
	float av = TMathUtil<float>::Clamp((l0 * l0 + l1 * l1 - l * l) / (2.0f * l0 * l1), -1.0f, 1.0f);
	float bv = TMathUtil<float>::Clamp((l0 * l0 + l * l - l1 * l1) / (2.0f * l0 * l), -1.0f, 1.0f);

	float a = TMathUtil<float>::ACos(av) + PI;
	float b = TMathUtil<float>::ACos(bv);

	a = a * 180.f / PI;
	b = b * 180.f / PI;

	FVector pointOnPlane = start;
	FVector es = end - start;
	es.Normalize();
	FVector planNormal = direction;//FVector::CrossProduct(es, direction);
	planNormal.Normalize();

	FVector p0 = start;
	FVector p1 = p0 + es.RotateAngleAxis(b, planNormal) * l0;
	FVector p2 = p1 + es.RotateAngleAxis(b + a, planNormal) * l1;

	outNodes.Add(p0);
	outNodes.Add(p1);
	outNodes.Add(p2);
}

// Called when the game starts or when spawned
void ALineActor::BeginPlay()
{
	Super::BeginPlay();
		
	/*LineActor =  GetWorld()->SpawnActor<AUnitLineActor>(AUnitLineActor::StaticClass());

	LineActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);*/
}

// Called every frame
void ALineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

