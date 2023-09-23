#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSTypes.generated.h"

UENUM(BlueprintType)
enum class EInvetoryItemType : uint8
{
    SPHERE = 0 UMETA(DisplayName = "My cool Sphere"),
    CUBE UMETA(DisplayName = "My cool Cube"),
    CYLIDER,
    CONE
};

USTRUCT(BlueprintType)
struct FInvetoryData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInvetoryItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    int32 Score;

    FString ToString() const { return *FString::Printf(TEXT("(Type=%i,Score=%i)"), Type, Score); }
};

USTRUCT(BlueprintType)
struct FHealthData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0"))
    float MaxHealth{100.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
    float HealModyfire{5.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "Seconds"))
    float HealRate{0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "-1.0"))
    float HealDelay{-1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Units = "Seconds"))
    float LifeSpan{5.0f};

    FString ToString() const
    {
        return *FString::Printf(TEXT("(MaxHealth=%f,HealModyfire=%f,HealRate=%f,HealDelay=%f,LifeSpan=%f)"),  //
            MaxHealth,                                                                                        //
            HealModyfire,                                                                                     //
            HealRate,                                                                                         //
            HealDelay,                                                                                        //
            LifeSpan                                                                                          //
        );                                                                                                    //
    }
};