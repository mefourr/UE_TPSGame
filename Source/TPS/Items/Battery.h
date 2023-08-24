// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace TPS
{

class TPS_API Battery
{
public:
    Battery() = default;
    Battery(float PercentIn);

    void Charge();
    void Uncharge();

    float GetPercent() const;
    FColor GetColor() const;
    FString ToString() const;
    float GetChargeAmount() const { return ChargeAmount; }

    bool operator>=(const Battery& rhs) const { return GetPercent() >= rhs.GetPercent(); }
    bool operator<(const Battery& rhs) const { return GetPercent() < rhs.GetPercent(); }
    bool operator==(const Battery& rhs) const { return FMath::IsNearlyEqual(GetPercent(), rhs.GetPercent()); }

private:
    const float ChargeAmount = 0.1f;

    float Percent{1.0f};
    void SetPercent(float PercentIn);
};
}  // namespace TPS