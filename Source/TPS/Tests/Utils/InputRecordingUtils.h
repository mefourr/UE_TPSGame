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
struct FActionData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    FName Name;

    UPROPERTY()
    FKey key;

    UPROPERTY()
    bool State;
};

USTRUCT()
struct FBindingsData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
    TArray<FAxisData> AxisValues;

    UPROPERTY()
    TArray<FActionData> ActionVlue;

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