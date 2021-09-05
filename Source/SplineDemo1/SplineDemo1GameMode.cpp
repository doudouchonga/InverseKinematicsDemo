// Copyright Epic Games, Inc. All Rights Reserved.

#include "SplineDemo1GameMode.h"
#include "SplineDemo1Character.h"
#include "UObject/ConstructorHelpers.h"

ASplineDemo1GameMode::ASplineDemo1GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
