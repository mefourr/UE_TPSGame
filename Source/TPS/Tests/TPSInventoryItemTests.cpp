// Copyright EpicGames.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSInventoryItemTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

#include "TPS/Items/TPSInvetoryItem.h"
#include "Engine/World.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppAcotrCantBeCreated, "TPSGame.Items.Inventory.CppAcotrCantBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectrly, "TPSGame.Items.Inventory.BlueprintShouldBeSetupCorrectrly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

/*
    References:
    /Script/Engine.World'/Game/Tests/EmptyTestLevel.EmptyTestLevel'
    /Script/Engine.Blueprint'/Game/Invetory/BP_TPSInvetoryItem.BP_TPSInvetoryItem'
*/

namespace
{
constexpr char* InventoryItemBPName = "/Script/Engine.Blueprint'/Game/Invetory/BP_TPSInvetoryItem.BP_TPSInvetoryItem'";

}  // namespace

using namespace TPS::Test;

bool FCppAcotrCantBeCreated::RunTest(const FString& Parameters)
{
    const FString ExpectedWarningMsg =
        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSInvetoryItem::StaticClass()->GetName());
    AddExpectedError(ExpectedWarningMsg, EAutomationExpectedErrorFlags::Exact);

    LevelScope{"/Game/Tests/EmptyTestLevel"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};  // Set location for Inventory Item
    const ATPSInvetoryItem* InvItem = World->SpawnActor<ATPSInvetoryItem>(ATPSInvetoryItem::StaticClass(), InitialTransform);
    if (!TestNull("Inventory item exists", InvItem)) return false;

    return true;
}

bool FBlueprintShouldBeSetupCorrectrly::RunTest(const FString& Parameters)
{
    LevelScope{"/Game/Tests/EmptyTestLevel"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};  // Set location for Inventory Item
    const ATPSInvetoryItem* InvItem = CreateBlueprint<ATPSInvetoryItem>(World, InventoryItemBPName, InitialTransform);
    if (!TestNotNull("Inventory Item exists", InvItem)) return false;

    const auto CollisionComp = InvItem->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("Collision Component exists", CollisionComp)) return false;

    TestTrueExpr(CollisionComp->GetUnscaledSphereRadius() >= 30.0f);
    TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
    TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn) == ECollisionResponse::ECR_Overlap);
    TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
    TestTrueExpr(InvItem->GetRootComponent() == CollisionComp);

    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(EElem) == ECollisionResponse::ECR_Overlap);
    }
    ENUM_LOOP_END

    const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("TextRender Component exists", TextRenderComp)) return false;

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("StaticMesh Component exists", StaticMeshComp)) return false;

    TestTrueExpr(StaticMeshComp->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

#endif
