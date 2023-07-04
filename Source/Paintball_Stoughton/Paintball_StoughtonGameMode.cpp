// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "Paintball_StoughtonGameMode.h"
#include "Paintball_StoughtonHUD.h"
#include "Paintball_StoughtonCharacter.h"
#include "UObject/ConstructorHelpers.h"

APaintball_StoughtonGameMode::APaintball_StoughtonGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = APaintball_StoughtonHUD::StaticClass();
}
