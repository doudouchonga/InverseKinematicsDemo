// Fill out your copyright notice in the Description page of Project Settings.


#include "LineActor.h"
#include "Engine/StaticMeshActor.h"
#include "UnitLineActor.h"
#include "Materials/Material.h"
#include "../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"

// Sets default values
ALineActor::ALineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("mesh"));
	MeshMaterial = LoadObject<UMaterial>(NULL, TEXT("/Game/StarterContent/Materials/M_Wood_Floor_Walnut_Worn"), NULL, LOAD_None, NULL);
	
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

