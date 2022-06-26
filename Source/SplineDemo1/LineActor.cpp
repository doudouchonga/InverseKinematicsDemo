// Fill out your copyright notice in the Description page of Project Settings.


#include "LineActor.h"
#include "Engine/StaticMeshActor.h"
#include "UnitLineActor.h"
#include "Materials/Material.h"
#include "../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include "../Plugins/Experimental/GeometryProcessing/Source/GeometricObjects/Public/MathUtil.h"

#include <type_traits>

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

void ALineActor::UpdateCCDIK(TArray<FVector> nodes, FVector targetPos, FVector direction, TArray<FVector>& OutNodes)
{
	int TotalNum = nodes.Num();
	if (TotalNum == 0)
	{
		return;
	}

	if (UpdateStep < 0)
	{
		OutNodes = nodes;
		return;
	}

	int iteraterCount = 0;
	
	TArray<FVector> rets = nodes;
	
	for (int i = TotalNum - 1; i >= 0; --i)
	{
		FVector EndEffectPos = rets[TotalNum - 1];

		float error = (targetPos - EndEffectPos).Size();
		if (error < 0.0001f)
		{
			break;
		}
		FVector p0 = i > 0 ? rets[i - 1] : FVector(0);
		FVector p1 = rets[i];

		FVector v0 = EndEffectPos - p0;
		FVector v1 = targetPos - p0;

		v0.Normalize();
		v1.Normalize();

		float dot = FVector::DotProduct(v0, v1);
		dot = TMathUtil<float>::Clamp(dot, -1, 1);
		float ang = FMath::RadiansToDegrees(TMathUtil<float>::ACos(dot));

		if (FMath::Abs(ang) > 0.1f)
		{
			FVector rotAxis = FVector::CrossProduct(v0, v1);
			if (rotAxis.SizeSquared() > 0)
			{
				rotAxis.Normalize();

				FVector es = p1 - p0;
				// 
				FVector newPos = p0 + es.RotateAngleAxis(ang, rotAxis);
				// 
				UpdatePoints(rets, i, newPos);
				
			}

			// Bounce back to end
			if (UseBounce)
			{
				i = TotalNum;
			}
			
		}

		// For DEBUG Update step
		if ((TotalNum - 1 - i) == UpdateStep)
		{
			break;
		}
	}

	OutNodes = rets;
	//for (FVector& v : rets)
	//{
	//	OutNodes.Add(v);
	//}

}

void ALineActor::UpdatePoints(TArray<FVector>& nodes, int k, FVector NewPos)
{
	if (k >= 0 && k < nodes.Num())
	{
		FVector CurPos = nodes[k];
		if (k < nodes.Num() - 1)
		{
			// ���Ÿ�����һ��λ�õĵ㣬���򱣳ֲ���
			FVector NextPos = nodes[k + 1];
			FVector es = NextPos - CurPos;
			nodes[k] = NewPos;
			FVector NextNewPos = NewPos + es;
			UpdatePoints(nodes, k+1, NextNewPos);
		}
		else
		{
			nodes[k] = NewPos;
		}
	}
}

void ALineActor::OnConstruction(const FTransform& Transform)
{
	if (UBlueprint* bl = Cast<UBlueprint>(GetClass()->ClassGeneratedBy))
	{
		// TArray<UEdGraph*> UbergraphPages;
		for (UEdGraph* graph : bl->UbergraphPages)
		{
			// TArray<class UEdGraphNode*> Nodes;
			for (UEdGraphNode* node : graph->Nodes)
			{
				FGuid guid = node->NodeGuid;
				TArray<UEdGraphPin*> t = node->GetAllPins();
				for (UEdGraphPin * p : t)
				{
				}
			}
		}
	}
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


ATestActor::ATestActor()
{
	
}

void ATestActor::BeginPlay()
{
	
}

