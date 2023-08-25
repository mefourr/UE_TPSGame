// My game copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPS/TPSTypes.h"
#include "TPSInvetoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TPS_API UTPSInvetoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSInvetoryComponent();

    bool TryToAddItem(const FInvetoryData& Data);

    UFUNCTION(BlueprintCallable)
    int32 GetInventoryAmoundByType(EInvetoryItemType Type) const;

protected:
    virtual void BeginPlay() override;

private:
    TMap<EInvetoryItemType, int32> Inventory;
};
