// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealTutorialGameMode.h"
#include "UnrealTutorialCharacter.h"
#include "Character/Player/UTCharacterPlayer.h"
#include "UObject/ConstructorHelpers.h"

AUnrealTutorialGameMode::AUnrealTutorialGameMode()
{
	DefaultPawnClass = AUTCharacterPlayer::StaticClass();
}
