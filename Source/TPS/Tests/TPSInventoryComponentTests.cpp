// Copyright EpicGames.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeScoreShouldntBeAdded, "TPSGame.Components.Inventory.NegativeScoreShouldntBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoreShouldBeAdded, "TPSGame.Components.Inventory.PositiveScoreShouldBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScoreMoreThanLimitCantBeAdded, "TPSGame.Components.Inventory.ScoreMoreThanLimitCantBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
class UTPSInvetoryComponentTestable : public UTPSInvetoryComponent
{
public:
    void SetLimits(const TMap<EInvetoryItemType, int32>& Limits) { InventoryLimits = Limits; }
};

TMap<EInvetoryItemType, int32> InitLimits(UTPSInvetoryComponentTestable* InvComp, int32 LimitValue)
{
    TMap<EInvetoryItemType, int32> InvLimits;
    ENUM_LOOP_START(EInvetoryItemType, EElem)
    InvLimits.Add(EElem, LimitValue);
    ENUM_LOOP_END
    InvComp->SetLimits(InvLimits);

    return InvLimits;
}
}  // namespace

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

bool FNegativeScoreShouldntBeAdded::RunTest(const FString& Parameters)
{
    UTPSInvetoryComponentTestable* InvComp = NewObject<UTPSInvetoryComponentTestable>();
    if (!TestNotNull("Inventory component exist", InvComp)) return false;

    // Set limits
    InitLimits(InvComp, 100);

    const int32 NegativeScoreAmount = -2;
    const int32 InitialScore = 5;

    ENUM_LOOP_START(EInvetoryItemType, EElem)
    // set positive value
    TestTrueExpr(InvComp->TryToAddItem({EElem, InitialScore}));
    TestTrueExpr(InvComp->GetInventoryAmoundByType(EElem) == InitialScore);

    // set negative value
    TestTrueExpr(!InvComp->TryToAddItem({EElem, NegativeScoreAmount}));
    TestTrueExpr(InvComp->GetInventoryAmoundByType(EElem) == InitialScore);
    ENUM_LOOP_END

    return true;
}

bool FPositiveScoreShouldBeAdded::RunTest(const FString& Parameters)
{
    UTPSInvetoryComponentTestable* InvComp = NewObject<UTPSInvetoryComponentTestable>();
    if (!TestNotNull("Inventory component exist", InvComp)) return false;

    const auto InvLimits = InitLimits(InvComp, 100);

    ENUM_LOOP_START(EInvetoryItemType, EElem)
    for (int32 i = 0; i < InvLimits[EElem]; ++i)
    {
        TestTrueExpr(InvComp->TryToAddItem({EElem, 1}));
        TestTrueExpr(InvComp->GetInventoryAmoundByType(EElem) == i + 1);
    }
    ENUM_LOOP_END

    return true;
}

bool FScoreMoreThanLimitCantBeAdded::RunTest(const FString& Parameters)
{
    AddInfo("Test a value beyond of limit");

    UTPSInvetoryComponentTestable* InvComp = NewObject<UTPSInvetoryComponentTestable>();
    if (!TestNotNull("Inventory component exist", InvComp)) return false;

    const int32 ScoreLimit = 100;
    InitLimits(InvComp, ScoreLimit);

    TestTrueExpr(InvComp->TryToAddItem({EInvetoryItemType::SPHERE, 10}));
    TestTrueExpr(InvComp->GetInventoryAmoundByType(EInvetoryItemType::SPHERE) == 10);

    TestTrueExpr(!InvComp->TryToAddItem({EInvetoryItemType::SPHERE, ScoreLimit + 1}));
    TestTrueExpr(InvComp->GetInventoryAmoundByType(EInvetoryItemType::SPHERE) == 10);

    return true;
}

#endif
