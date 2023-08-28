// Copyright EpicGames.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSInventoryComponentTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Components/TPSInvetoryComponent.h"
#include "TPS/TPSTypes.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCouldBeCreated, "TPSGame.Components.Inventory.ComponentCouldBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryShouldBeZeroByDefault, "TPSGame.Components.Inventory.InventoryShouldBeZeroByDefault",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

bool FComponentCouldBeCreated::RunTest(const FString& Parameters)
{
    const UTPSInvetoryComponent* InvComp = NewObject<UTPSInvetoryComponent>();
    if (!TestNotNull("Inventory component exist", InvComp)) return false;

    return true;
}

bool FInventoryShouldBeZeroByDefault::RunTest(const FString& Parameters)
{
    const UTPSInvetoryComponent* InvComp = NewObject<UTPSInvetoryComponent>();
    if (!TestNotNull("Inventory component exist", InvComp)) return false;

    ENUM_LOOP_START(EInvetoryItemType, EElem)
    TestTrueExpr(InvComp->GetInventoryAmoundByType(EElem) == 0);
    ENUM_LOOP_END

    return true;
}

#endif
