// Copyright EpicGames.

#if WITH_AUTOMATION_TESTS

#include "TPS/Tests/TPSCharacterTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Engine/World.h"
#include "TPS/TPSCharacter.h"
#include "TPS/TPSTypes.h"
#include "Tests/AutomationCommon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "kismet/GameplayStatics.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSCharacterTests, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthMightBeChangedWithDamage, "TPSGame.Character.HealthMightBeChangedWithDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleWait, "TPSGame.LatentCommand.SimpleWait",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleLog, "TPSGame.LatentCommand.SimpleLog",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandOpenCloseMap, "TPSGame.LatentCommand.OpenCloseMap",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled, "TPSGame.Character.CharacterCanBeKilled",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoHealShouldResotreHealth, "TPSGame.Character.AutoHealShouldResotreHealth",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
constexpr char* CharacterTestBPName = "/Script/Engine.Blueprint'/Game/Tests/BP_TPSTestCharacter.BP_TPSTestCharacter'";
}  // namespace

bool FHealthMightBeChangedWithDamage::RunTest(const FString& Parameters)
{
    LevelScope{"/Game/Tests/EmptyTestLevel"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    // delayed call
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    // call begin play
    Character->FinishSpawning(InitialTransform);

    const float DamageAmount{100.0f};
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    return true;
}

/*
 *  LatentCommand
 *  Lets you make tests which depend on the game, them a Tick
 */

bool FLatentCommandSimpleWait::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand{3.0f});

    return true;
}

// DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FTPSLatentLogCommonad, FString, LogMessage);

class FTPSLatentLogCommonad : public IAutomationLatentCommand  // instead of the macro above
{
public:
    FTPSLatentLogCommonad(FString InputParam) : LogMessage(InputParam){};
    virtual ~FTPSLatentLogCommonad(){};
    virtual bool Update() override;

private:
    FString LogMessage;
};

bool FTPSLatentLogCommonad::Update()
{
    UE_LOG(LogTPSCharacterTests, Display, TEXT("%s"), *LogMessage);
    return true;
}

bool FLatentCommandSimpleLog::RunTest(const FString& Parameters)
{
    UE_LOG(LogTPSCharacterTests, Display, TEXT("Log 1"));

    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        []()
        {
            UE_LOG(LogTPSCharacterTests, Display, TEXT("Latent log 1"));
            return true;
        }));
    UE_LOG(LogTPSCharacterTests, Display, TEXT("Log 2"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSLatentLogCommonad("Latent log 2"));
    UE_LOG(LogTPSCharacterTests, Display, TEXT("Log 3"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSLatentLogCommonad("Latent log 3"));

    return true;
}

bool FLatentCommandOpenCloseMap::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope{"/Game/Tests/EmptyTestLevel"};
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand{5.0f});

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterDestroyedLaltentCommand, ATPSCharacter*, Character, float, LifeSpan);

bool FCharacterDestroyedLaltentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= LifeSpan)
    {
        if (IsValid(Character))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Character must be killed %s"), *Character->GetName());
        }
        return true;
    }

    return false;
}

bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope{"/Game/Tests/EmptyTestLevel"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    // delayed call
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character)) return false;

    {
        TArray<AActor*> Pawns;
        UGameplayStatics::GetAllActorsOfClass(World, ATPSCharacter::StaticClass(), Pawns);

        const auto CurrentCharacter = Cast<ATPSCharacter>(Pawns[0]);
        if (!TestNotNull("CurrentCharacter exists", CurrentCharacter)) return false;

        const auto Controller = CurrentCharacter->GetController();
        if (!TestNotNull("Controller exists", Controller)) return false;

        Controller->UnPossess();
        Controller->Possess(Character);

        UE_LOG(LogTPSCharacterTests, Display, TEXT("Controller state: %s "), *Controller->GetStateName().ToString());
    }

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    HealthData.LifeSpan = 3.5f;
    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    // call begin play
    Character->FinishSpawning(InitialTransform);
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);

    // TODO: check controller state
    /*

    // APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    // if (!TestNotNull("PlayerController exists", PlayerController)) return false;
    // UE_LOG(LogTPSCharacterTests, Display, TEXT("StateName: %s ---"), *PlayerController->GetStateName().ToString());

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World, ATPSCharacter::StaticClass(), Pawns);

    UE_LOG(LogTPSCharacterTests, Display, TEXT(" Pawns.Num(): %i ---"), Pawns.Num());

    for (const auto& Pawn : Pawns)
    {
        const auto CharacterTest = Cast<ATPSCharacter>(Pawn);
        if (!CharacterTest) return false;

        const auto PlayerController = Cast<APlayerController>(CharacterTest->GetController());
        if (!TestNotNull("PlayerController exists", PlayerController)) return false;

        UE_LOG(LogTPSCharacterTests, Display, TEXT(" -- Get controller name: %s -- "), *CharacterTest->GetController()->GetName());
    }

    //..................................................................................




    */

    const auto KlllingDamageAmount = HealthData.MaxHealth;
    Character->TakeDamage(KlllingDamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Character has been killed", Character->GetHealthPercent(), 0.0f);

    TestTrueExpr(Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None);

    ENUM_LOOP_START(ECollisionChannel, EElem)
    if (EElem != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(Character->GetCapsuleComponent()->GetCollisionResponseToChannel(EElem) == ECollisionResponse::ECR_Ignore);
    }
    ENUM_LOOP_END

    TestTrueExpr(Character->GetMesh()->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics);
    TestTrueExpr(Character->GetMesh()->IsSimulatingPhysics());
    TestTrueExpr(FMath::IsNearlyEqual(Character->GetLifeSpan(), HealthData.LifeSpan));

    // ADD_LATENT_AUTOMATION_COMMAND(FCharacterDestroyedLaltentCommand(Character, HealthData.LifeSpan));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]()
        {
            if (IsValid(Character))
            {
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Character must be killed %s"), *Character->GetName());
            }
        },
        HealthData.LifeSpan));

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FAutomationCheck, ATPSCharacter*, Character, float, HealingDuration);

bool FAutomationCheck::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= HealingDuration)
    {
        if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Health hasn't been restored %f"), Character->GetHealthPercent());
        }
        return true;
    }

    return false;
}

bool FAutoHealShouldResotreHealth::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope{"/Game/Tests/EmptyTestLevel"};

    UWorld* World = GetTestGameWorld();
    if (!TestNotNull("World exists", World)) return false;

    const FTransform InitialTransform{FVector{0.0f, -240.0f, 110.0f}};
    // delayed call
    ATPSCharacter* Character = CreateBlueprintDeferred<ATPSCharacter>(World, CharacterTestBPName, InitialTransform);
    if (!TestNotNull("Character exists", Character)) return false;

    FHealthData HealthData;
    HealthData.MaxHealth = 200.0f;
    HealthData.HealModyfire = 10.0f;
    HealthData.HealRate = 0.5f;

    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});

    // call begin play
    Character->FinishSpawning(InitialTransform);

    const float DamageAmount{100.0f};
    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    Character->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
    TestEqual("Health was decreased", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    const float HealthDiff = HealthData.MaxHealth * (1.0f - Character->GetHealthPercent());
    const float HealingDuration = HealthData.HealRate * HealthDiff / HealthData.HealModyfire;

    // ADD_LATENT_AUTOMATION_COMMAND(FAutomationCheck(Character, HealingDuration));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]()
        {
            if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
            {
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Health hasn't been restored %f"), Character->GetHealthPercent());
            }
        },
        HealingDuration));

    return true;
}

#endif
