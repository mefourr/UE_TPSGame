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
#include "Misc/OutputDeviceNull.h"
#include "kismet/GameplayStatics.h"
#include "TPS/TPSCharacter.h"
#include "TPS/Components/TPSInvetoryComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppAcotrCantBeCreated, "TPSGame.Items.Inventory.CppAcotrCantBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectrly, "TPSGame.Items.Inventory.BlueprintShouldBeSetupCorrectrly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectrly, "TPSGame.Items.Inventory.InventoryDataShouldBeSetupCorrectrly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryCanBeTaken, "TPSGame.Items.Inventory.InventoryCanBeTaken",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEveryInvntoryMeshExist, "TPSGame.Items.Inventory.EveryInvntoryMeshExist",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

/*
    References:
    /Script/Engine.World'/Game/Tests/EmptyTestLevel.EmptyTestLevel'
    /Script/Engine.Blueprint'/Game/Invetory/BP_TPSInvetoryItem.BP_TPSInvetoryItem'
*/

namespace
{
constexpr char* InventoryItemBPName = "/Script/Engine.Blueprint'/Game/Invetory/BP_TPSInvetoryItem.BP_TPSInvetoryItem'";
constexpr char* InventoryItemBPTestName = "/Script/Engine.Blueprint'/Game/Tests/BP_Test_TPSInvetoryItem.BP_Test_TPSInvetoryItem'";

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

bool FInventoryDataShouldBeSetupCorrectrly::RunTest(const FString& Parameters)
{
    LevelScope{"/Game/Tests/EmptyTestLevel"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    ATPSInvetoryItem* InvItem = CreateBlueprint<ATPSInvetoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory Item exists", InvItem)) return false;

    FVector Vec{120.f, 130.f, 104.f};
    CallFuncByNameWithParams(InvItem, "SetTestData",
        {
            FString::FromInt(123),                                                    //
            FString::SanitizeFloat(232.71f),                                          //
            *FString("\"Suka\""),                                                     //
            *FString("Yes"),                                                          //
            *FString::Printf(TEXT("(X=%3.1f,Y=%3.1f,Z=%3.1f)"), Vec.X, Vec.Y, Vec.Z)  //
        });

    const FInvetoryData InvData{EInvetoryItemType::CYLIDER, 300};
    const FLinearColor Color = FLinearColor::Red;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    const auto TextRenderComp = InvItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("TextRender Component exists", TextRenderComp)) return false;

    TestTrueExpr(TextRenderComp->Text.ToString().Equals(FString::FromInt(InvData.Score)));
    TestTrueExpr(TextRenderComp->TextRenderColor == Color.ToFColor(true));

    const auto StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("StaticMesh Component exists", StaticMeshComp)) return false;

    const auto Material = StaticMeshComp->GetMaterial(0);
    if (!TestNotNull("Material exists", Material)) return false;

    FLinearColor MaterialCOlor;
    Material->GetVectorParameterValue(FHashedMaterialParameterInfo{"Color"}, MaterialCOlor);
    TestTrueExpr(MaterialCOlor == Color);

    return true;
}

bool FInventoryCanBeTaken::RunTest(const FString& Parameters)
{
    LevelScope{"/Game/Tests/EmptyTestLevel"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{1000.0f}};
    ATPSInvetoryItem* InvItem = CreateBlueprint<ATPSInvetoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory Item exists", InvItem)) return false;

    const FInvetoryData InvData{EInvetoryItemType::CYLIDER, 13};
    const FLinearColor Color = FLinearColor::Red;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSCharacter::StaticClass(), Pawns);
    if (!TestTrueExpr(Pawns.Num() == 1)) return false;

    const auto Character = Cast<ATPSCharacter>(Pawns[0]);
    if (!TestNotNull("ATPSCharacter exists", Character)) return false;

    const auto InvComp = Character->FindComponentByClass<UTPSInvetoryComponent>();
    if (!TestNotNull("UTPSInventoryComponent exists", InvComp)) return false;

    TestTrueExpr(InvComp->GetInventoryAmoundByType(InvData.Type) == 0);

    // character takes inventory item;
    Character->SetActorLocation(InvItem->GetActorLocation() /*InitialTransform.GetLocation()*/);

    TestTrueExpr(InvComp->GetInventoryAmoundByType(InvData.Type) == InvData.Score);

    TestTrueExpr(!IsValid(InvItem));

    TArray<AActor*> InvItems;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSInvetoryItem::StaticClass(), InvItems);
    TestTrueExpr(!InvItems.Num());

    return true;
}

bool FEveryInvntoryMeshExist::RunTest(const FString& Parameters)
{
    LevelScope{"/Game/Tests/EmptyTestLevel"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    ENUM_LOOP_START(EInvetoryItemType, EElem)

    const FTransform InitialTransform{FVector{100.0f * (Index + 1)}};
    ATPSInvetoryItem* InvItem = CreateBlueprint<ATPSInvetoryItem>(World, InventoryItemBPTestName, InitialTransform);
    if (!TestNotNull("Inventory Item exists", InvItem)) return false;

    const FInvetoryData InvData{EElem, 13};
    const FLinearColor Color = FLinearColor::Yellow;
    CallFuncByNameWithParams(InvItem, "SetInventoryData", {InvData.ToString(), Color.ToString()});

    const UStaticMeshComponent* StaticMeshComp = InvItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("StaticMesh Component exists", StaticMeshComp)) return false;

    const FString MeshMsg = FString::Printf(TEXT("Static mesh for %s exist"), *UEnum::GetValueAsString(EElem));
    TestNotNull(MeshMsg, (UStaticMesh*)StaticMeshComp->GetStaticMesh());

    ENUM_LOOP_END
    return true;
}

#endif