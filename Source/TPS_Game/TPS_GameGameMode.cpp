// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPS_GameGameMode.h"
#include "TPS_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATPS_GameGameMode::ATPS_GameGameMode() {
  // set default pawn class to our Blueprinted character
  static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
      TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
  if (PlayerPawnBPClass.Class != NULL) {
    DefaultPawnClass = PlayerPawnBPClass.Class;
  }
}
