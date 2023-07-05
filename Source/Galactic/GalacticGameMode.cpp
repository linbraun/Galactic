// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "GalacticGameMode.h"
#include "GalacticHUD.h"
#include "GalacticCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGalacticGameMode::AGalacticGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AGalacticHUD::StaticClass();
}
