#pragma once

#include "CoreMinimal.h"

namespace TPS
{
namespace Test
{
template <typename T1, typename T2>
struct TestPayload
{
    T1 TestValue;
    T2 ExpectedValue;
    float Tolerance = KINDA_SMALL_NUMBER;
};

#define ENUM_LOOP_START(TYPE, EnumElem)                                                                                                    \
    const UEnum* InvEnum = StaticEnum<TYPE>();                                                                                             \
    check(InvEnum);                                                                                                                        \
    for (int32 i = 0; i < InvEnum->NumEnums() - 1; ++i)                                                                                    \
    {                                                                                                                                      \
        const TYPE EnumElem = static_cast<TYPE>(i);
#define ENUM_LOOP_END }

}  // namespace Test

}  // namespace TPS