// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Battery.h"

using namespace TPS;

Battery::Battery(float PercentIn)
{
    SetPercent(PercentIn);
}

void Battery::Charge()
{
    SetPercent(GetPercent() + GetChargeAmount());
}

void Battery::Uncharge()
{
    SetPercent(GetPercent() - GetChargeAmount());
}

float Battery::GetPercent() const
{
    return Percent;
}

FColor Battery::GetColor() const
{
    if (GetPercent() > .5f) return FColor::Green;
    if (GetPercent() > .2f) return FColor::Yellow;

    return FColor::Red;
}

FString Battery::ToString() const
{
    return FString::Printf(TEXT("%i%%"), FMath::RoundToInt(GetPercent() * 100));
}

void TPS::Battery::SetPercent(float PercentIn)
{
    Percent = FMath::Clamp(PercentIn, 0.0f, 1.0f);
}