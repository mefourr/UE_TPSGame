// My game copyright

#include "Tests/Components/TPSInputRecordingComponent.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "TPS/Tests/Utils/JsonUtils.h"
#include "GameFramework/PlayerInput.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSInputRecording, All, All);

using namespace TPS::Test;

UTPSInputRecordingComponent::UTPSInputRecordingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTPSInputRecordingComponent::BeginPlay()
{
    Super::BeginPlay();

    const auto* Pawn = Cast<APawn>(GetOwner());
    check(Pawn);

    const auto* PlayerController = Pawn->GetController<APlayerController>();
    check(PlayerController);

    PlayerInput = PlayerController->PlayerInput;
    check(PlayerInput);

    check(GetOwner());
    check(GetWorld());
    check(GetOwner()->InputComponent);

    InputData.InitialTransform = GetOwner()->GetActorTransform();
    InputData.Bindings.Add(MakeBindingData());
}

void UTPSInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (!JsonUtils::WriteInputData(GenerateFilename(), InputData))
    {
        UE_LOG(LogTPSInputRecording, Error, TEXT("WriteInputData error"));
    }
}

void UTPSInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    InputData.Bindings.Add(MakeBindingData());
}

FBindingsData UTPSInputRecordingComponent::MakeBindingData() const
{
    FBindingsData BindingData;
    BindingData.WorldTime = GetWorld()->TimeSeconds;
    for (auto& AxisBinding : GetOwner()->InputComponent->AxisBindings)
    {
        BindingData.AxisValues.Add(FAxisData{AxisBinding.AxisName, AxisBinding.AxisValue});
    }

    for (int32 i = 0; i < GetOwner()->InputComponent->GetNumActionBindings(); ++i)
    {
        const auto Action = GetOwner()->InputComponent->GetActionBinding(i);
        const auto ActionKeys = PlayerInput->GetKeysForAction(Action.GetActionName());
        if (ActionKeys.Num() > 0)
        {
            const bool Pressed = PlayerInput->IsPressed(ActionKeys[0].Key);
            const bool State = (Pressed && Action.KeyEvent == EInputEvent::IE_Pressed) ||  //
                               (!Pressed && Action.KeyEvent == EInputEvent::IE_Released);
            BindingData.ActionVlue.Add(FActionData{Action.GetActionName(), ActionKeys[0].Key, State});
        }
    }

    return BindingData;
}

FString UTPSInputRecordingComponent::GenerateFilename() const
{
    FString SaveDir = FPaths::ProjectSavedDir();
    const FString Data = FDateTime::Now().ToString();
    return SaveDir.Append("/Tests/").Append(FileName).Append("_").Append(Data).Append(".json");
}
