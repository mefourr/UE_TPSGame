// Copyright EpicGames.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSGameplayTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Items/TPSInvetoryItem.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventroyItemCanBeTakenOnJump, "TPSGame.Gameplay.FInventroyItemCanBeTakenOnJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

bool FInventroyItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope{"/Game/Tests/InventroyTestLevel_1"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Charcter = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Charcter exists", Charcter)) return false;

    TArray<AActor*> InventroyItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInvetoryItem::StaticClass(), InventroyItems);
    if (!TestEqual("Only one item exists", InventroyItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));

    // Jump ...

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventroyItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInvetoryItem::StaticClass(), InventroyItems);
            TestTrueExpr(InventroyItems.Num() == 0);
        },
        2.0f));
    return true;
}

#endif
