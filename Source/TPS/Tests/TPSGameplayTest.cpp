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
#include "TestUtils.h"

#define UNDEFINEINDEX -1

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventroyItemCanBeTakenOnJump, "TPSGame.Gameplay.InventroyItemCanBeTakenOnJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventroyItemCantBeTakenOnJumpIfTooHigh, "TPSGame.Gameplay.InventroyItemCantBeTakenOnJumpIfTooHigh",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsAreTakenOnMovement, "TPSGame.Gameplay.AllItemsAreTakenOnMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{

}  // namespace

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, ACharacter*, Character);

bool FJumpLatentCommand::Update()
{
    if (!Character) return false;
    const int32 ActionIndex = GetActionBindingIndexByName(Character->InputComponent, "Jump", EInputEvent::IE_Pressed);
    if (ActionIndex != UNDEFINEINDEX)
    {
        const auto JumpAction = Character->InputComponent->GetActionBinding(ActionIndex);
        JumpAction.ActionDelegate.Execute(EKeys::SpaceBar);
    }
    return true;
}

bool FInventroyItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    // /Game/Tests/InventroyTestLevel_2'
    const auto Level = LevelScope{"/Game/Tests/InventroyTestLevel_1"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Charcter exists", Character)) return false;

    TArray<AActor*> InventroyItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInvetoryItem::StaticClass(), InventroyItems);
    if (!TestEqual("Only one item exists", InventroyItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    // Jump ...
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));

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

bool FInventroyItemCantBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope{"/Game/Tests/InventroyTestLevel_2"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Charcter exists", Character)) return false;

    TArray<AActor*> InventroyItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInvetoryItem::StaticClass(), InventroyItems);
    if (!TestEqual("Only one item exists", InventroyItems.Num(), 1)) return false;

    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    // Jump ...
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));

    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventroyItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInvetoryItem::StaticClass(), InventroyItems);
            TestTrueExpr(InventroyItems.Num() == 1);
        },
        2.0f));
    return true;
}

bool FAllItemsAreTakenOnMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope{"/Game/Tests/InventroyTestLevel_3"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Charcter exists", Character)) return false;

    TArray<AActor*> InventroyItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInvetoryItem::StaticClass(), InventroyItems);
    TestTrueExpr(InventroyItems.Num() == 7);

    const int32 MoveForwardIndex = GetAxisBindingIndexByName(Character->InputComponent, "MoveForward");
    TestTrueExpr(MoveForwardIndex != INDEX_NONE);
    const int32 MoveRightIndex = GetAxisBindingIndexByName(Character->InputComponent, "MoveRight");
    TestTrueExpr(MoveRightIndex != INDEX_NONE);

    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilCommand(                                                       //
        [=]() { Character->InputComponent->AxisBindings[MoveForwardIndex].AxisDelegate.Execute(1.0f); },  //
        [=]() {},                                                                                         //
        3.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilCommand(                                                     //
        [=]() { Character->InputComponent->AxisBindings[MoveRightIndex].AxisDelegate.Execute(1.0f); },  //
        [=]() {},                                                                                       //
        2.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [=]()
        {
            TArray<AActor*> InventroyItems;
            UGameplayStatics::GetAllActorsOfClass(World, ATPSInvetoryItem::StaticClass(), InventroyItems);
            TestTrueExpr(InventroyItems.Num() == 0);
            return true;
        }));
    return true;
}

#endif
