#pragma once
#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

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

#define ENUM_LOOP_START(TYPE, EnumElem)                                        \
    for (int32 index = 0; index < StaticEnum<TYPE>()->NumEnums() - 1; ++index) \
    {                                                                          \
        const TYPE EnumElem = static_cast<TYPE>(index);
#define ENUM_LOOP_END }

}  // namespace Test

}  // namespace TPS

#endif