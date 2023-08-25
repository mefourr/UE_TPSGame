// My game copyright

#include "Components/TPSInvetoryComponent.h"

UTPSInvetoryComponent::UTPSInvetoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTPSInvetoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UTPSInvetoryComponent::TryToAddItem(const FInvetoryData& Data)
{
    if (Data.Score < 0) return false;

    if (!Inventory.Contains(Data.Type))
    {
        Inventory.Add(Data.Type, 0);
    }

    Inventory[Data.Type] += Data.Score;

    return true;
}

int32 UTPSInvetoryComponent::GetInventoryAmoundByType(EInvetoryItemType Type) const
{
    return Inventory.Contains(Type) ? Inventory[Type] : 0;
}