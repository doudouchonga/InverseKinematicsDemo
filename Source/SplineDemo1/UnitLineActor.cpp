// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitLineActor.h"

// Sets default values
AUnitLineActor::AUnitLineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TankStaticMesh(TEXT("/Engine/BasicShapes/Cylinder"));
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetStaticMesh(TankStaticMesh.Object);
	StaticMeshComponent->Mobility = EComponentMobility::Movable;
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->bUseDefaultCollision = true;

	RootComponent = StaticMeshComponent;

	SetActorScale3D(FVector(0.1, 0.1, 1.0F));
}

// Called when the game starts or when spawned
void AUnitLineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnitLineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

