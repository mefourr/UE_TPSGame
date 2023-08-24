// Fill out your copyright notice in the Description page of Project Settings.
#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/SienceFuncLibTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Science/ScienceFuncLib.h"
#include "Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple, "TPSGame.Sience.Fibonacci.Simple",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress, "TPSGame.Sience.Fibonacci.Stress",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::StressFilter | EAutomationTestFlags::LowPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasErrors, "TPSGame.Sience.Fibonacci.LogHasErrors",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

bool FFibonacciSimple::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci simple testing");

    // 0 1 1 2 3 5 8 ...

    typedef TArray<TestPayload<int32, int32>> TestFibonacciSimple;

    const TestFibonacciSimple TestData{{0, 0}, {1, 1}, {2, 1}, {3, 2}, {4, 3}, {5, 5}, {6, 8}};

    for (const auto& Data : TestData)
    {
        // TestTrueExpr(UScienceFuncLib::Fibonacci(Data.TestValue) ==
        // Data.ExpectedValue);

        const FString InfoString = FString::Printf(TEXT("TestValue: [%i] | ExpectedValue: [%i]"), Data.TestValue, Data.ExpectedValue);
        TestEqual(InfoString, UScienceFuncLib::Fibonacci(Data.TestValue), Data.ExpectedValue);
    }

    return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci stress testing");

    /*
    for (int32 i = 2; i < 40; ++i)
    {
        TestTrueExpr(UScienceFuncLib::Fibonacci(i) ==  //
                     UScienceFuncLib::Fibonacci(i - 1) +
    UScienceFuncLib::Fibonacci(i - 2)); } // 1.672s
    */

    int32 PrevValue = 1, PrevPrevValue = 0;

    for (int32 i = 2; i < 40; ++i)
    {
        const int32 CurrentValue = UScienceFuncLib::Fibonacci(i);
        TestTrueExpr(CurrentValue == PrevValue + PrevPrevValue);

        PrevPrevValue = PrevValue;
        PrevValue = CurrentValue;
    }  // 0.784s

    return true;
}

bool FFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci negative numbers on input produces an error");

    AddExpectedError("Ivalid input for Fibonacci", EAutomationExpectedErrorFlags::Contains);
    UScienceFuncLib::Fibonacci(-10);

    return true;
}

#endif
