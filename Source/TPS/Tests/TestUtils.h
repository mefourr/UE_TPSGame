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
}  // namespace Test

}  // namespace TPS