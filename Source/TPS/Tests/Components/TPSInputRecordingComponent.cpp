// My game copyright

#include "Tests/Components/TPSInputRecordingComponent.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "TPS/Tests/Utils/JsonUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSInputRecording, All, All);

using namespace TPS::Test;

UTPSInputRecordingComponent::UTPSInputRecordingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTPSInputRecordingComponent::BeginPlay()
{
    Super::BeginPlay();

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
    return BindingData;
}

FString UTPSInputRecordingComponent::GenerateFilename() const
{
    return FPaths::GameSourceDir().Append("TPS/Tests/Data/CharacterTestInput.json");
}
