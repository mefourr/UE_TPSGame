// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InputRecordingUtils.generated.h"

USTRUCT()
struct FAxisData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FName Name;

    UPROPERTY()
    float Value;
};

USTRUCT()
struct FBindingsData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FAxisData> AxisValues;

    // TODO: ActionData

    UPROPERTY()
    float WorldTime{0.0f};
};

USTRUCT()
struct FInputData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FBindingsData> Bindings;

    UPROPERTY()
    FTransform InitialTransform;
};