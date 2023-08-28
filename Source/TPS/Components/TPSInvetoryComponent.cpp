// My game copyright

#include "Components/TPSInvetoryComponent.h"

UTPSInvetoryComponent::UTPSInvetoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTPSInvetoryComponent::BeginPlay()
{
    Super::BeginPlay();

#if !UE_BUILD_SHIPPING
    const UEnum* InvEnum = StaticEnum<EInvetoryItemType>();
    check(InvEnum);

    for (int32 i = 0; i < InvEnum->NumEnums() - 1; ++i)
    {
        const EInvetoryItemType EnumElem = static_cast<EInvetoryItemType>(i);

        const FString EnumElemName = UEnum::GetValueAsString(EnumElem);
        const bool LimitCheckCond = InventoryLimits.Contains(EnumElem) && InventoryLimits[EnumElem] >= 0;

        checkf(LimitCheckCond, TEXT("Limit for %s doesn't exist or less than zero"), *EnumElemName);
    }
#endif
}

bool UTPSInvetoryComponent::TryToAddItem(const FInvetoryData& Data)
{
    if (Data.Score < 0) return false;

    if (!Inventory.Contains(Data.Type))
    {
        Inventory.Add(Data.Type, 0);
    }

    const auto NextScore = Inventory[Data.Type] + Data.Score;
    if (NextScore > InventoryLimits[Data.Type]) return false;

    Inventory[Data.Type] = NextScore;

    return true;
}

int32 UTPSInvetoryComponent::GetInventoryAmoundByType(EInvetoryItemType Type) const
{
    return Inventory.Contains(Type) ? Inventory[Type] : 0;
}