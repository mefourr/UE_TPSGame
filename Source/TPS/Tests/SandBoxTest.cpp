// Fill out your copyright notice in the Description page of Project Settings.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "TPS/Tests/SandBoxTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "TPSGame.Math.MaxInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "TPSGame.Math.Sqrt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMin, "TPSGame.Math.MinInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSin, "TPSGame.Math.Sin",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("Max [int] func testing");

    typedef TArray<TestPayload<TInterval<int32>, int32>> MaxIntTestPayload;

    // clang-format off
    const MaxIntTestPayload TestData
    {       
        {{13, 25}, 25},
        {{25, 25}, 25},
        {{0, 123}, 123},
        {{0, 0}, 0},
        {{-2345, 0}, 0},
        {{-45, -67}, -45},
        {{-9, -9}, -9},
    };
    // clang-format on

    for (const auto& Data : TestData)
    {
        TestTrueExpr(FMath::Max(Data.TestValue.Max, Data.TestValue.Min) == Data.ExpectedValue);
    }

    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    AddInfo("Sqrt func testing");

    typedef TArray<TestPayload<float, float>> SqrtFibonacciSimple;

    // clang-format off
    const SqrtFibonacciSimple TestData
    {
        {4.0f, 2.0f}, 
        {3.0f, 1.7f, 0.1f}, 
        {3.0f, 1.73f, 0.01f}, 
        {3.0f, 1.73205f, 1.e-5f}
    };
    // clang-format on

    for (const auto& Data : TestData)
    {
        const FString InfoString = FString::Printf(TEXT("TestValue: [%f] | ExpectedValue: [%f]"), Data.TestValue, Data.ExpectedValue);

        const bool IsEqual = FMath::IsNearlyEqual(FMath::Sqrt(Data.TestValue), Data.ExpectedValue, Data.Tolerance);

        TestTrue(InfoString, IsEqual);
    }

    return true;
}

bool FMathMin::RunTest(const FString& Parameters)
{
    AddInfo("Int min func testing");

    /*
     *  2 : 6
     *  -5 : 2
     *  -2 : -2
     *  0 : 0
     *  -0 : 0
     *  4.4 : 2
     *  -5 : -7
     */

    TestTrue("index: [0]", FMath::Min(2, 6) == 2);
    TestTrue("index: [1]", FMath::Min(-5, 2) == -5);
    TestTrue("index: [2]", FMath::Min(-2, -2) == -2);
    TestTrue("index: [3]", FMath::Min(0, 0) == 0);
    TestTrue("index: [4]", FMath::Min(-0, 0) == 0);
    TestTrue("index: [5]", FMath::Min(4, 2) == 2);
    TestTrue("index: [6]", FMath::Min(-5, -7) == -7);

    return true;
}

bool FMathSin::RunTest(const FString& Parameters)
{
    AddInfo("Sin func testing");

    typedef float Degrees;
    typedef TArray<TestPayload<Degrees, float>> SinFibonacciSimple;

    // clang-format off
    const SinFibonacciSimple TestData 
    {
        { Degrees{0.00f},   0.0f    }, 
        { Degrees{30.0f},   0.5f    }, 
        { Degrees{45.0f},   0.707f  }, 
        { Degrees{60.0f},   0.866f  },
        { Degrees{90.0f},   1.0f    }
    };
    // clang-format on

    for (const auto& Data : TestData)
    {
        const auto Radians = FMath::DegreesToRadians(Data.TestValue);
        TestTrueExpr(FMath::IsNearlyEqual(FMath::Sin(Radians), Data.ExpectedValue, 0.001f));
    }

    return true;
}

#endif