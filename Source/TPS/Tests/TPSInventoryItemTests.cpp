// Copyright EpicGames.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "TPS/Tests/TPSInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "TPS/Items/TPSInvetoryItem.h"
#include "Engine/World.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppAcotrCanBeCreated, "TPSGame.Items.Inventory.CppAcotrCanBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

// /Script/Engine.World'/Game/Tests/EmptyTestLevel.EmptyTestLevel'

namespace
{
UWorld* GetTestGameWorld()
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World())
        {
            return Context.World();
        }
    }
    return nullptr;
}

class LevelScope
{
public:
    LevelScope(const FString MapName) { AutomationOpenMap(MapName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};
}  // namespace

bool FCppAcotrCanBeCreated::RunTest(const FString& Parameters)
{
    LevelScope{"/Game/Tests/EmptyTestLevel"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};  // Set location for Inventory Item
    const ATPSInvetoryItem* InvItem = World->SpawnActor<ATPSInvetoryItem>(ATPSInvetoryItem::StaticClass(), InitialTransform);
    if (!TestNotNull("Inventory item exists", InvItem)) return false;

    return true;
}

#endif
