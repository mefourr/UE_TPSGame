#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSTypes.generated.h"

UENUM(BlueprintType)
enum class EInvetoryItemType : uint8
{
    SPHERE = 0,
    CUBE,
    CYLIDER
};

USTRUCT(BlueprintType)
struct FInvetoryData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInvetoryItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    int32 Score;
};