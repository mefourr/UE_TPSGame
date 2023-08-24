
#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "TPS/Tests/BatteryTest.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TPS/Items/Battery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatteryTests, "TPSGame.Items.Battery",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBatteryTests::RunTest(const FString& Parameters)
{
    using namespace TPS;

    AddInfo("Battery with default ctor");
    const Battery BatteryDefault;
    TestTrueExpr(FMath::IsNearlyEqual(BatteryDefault.GetPercent(), 1.0f));
    TestTrueExpr(BatteryDefault.GetColor() == FColor::Green);
    TestTrueExpr(BatteryDefault.ToString().Equals("100%"));

    AddInfo("Battery with custom ctor");

    // clang-format off
    const auto BatteryTestFunc = [this](float Percent, const FColor Color, const FString PercentString) 
    { 
        const Battery BatteryObject{Percent};
   
        TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), FMath::Clamp(Percent, 0.0f, 1.0f)));
        TestTrueExpr(BatteryObject.GetColor() == Color);
        TestTrueExpr(BatteryObject.ToString().Equals(PercentString));
    };
    // clang-format on

    BatteryTestFunc(1.0f, FColor::Green, "100%");
    BatteryTestFunc(.45f, FColor::Yellow, "45%");
    BatteryTestFunc(.12f, FColor::Red, "12%");
    BatteryTestFunc(3000.0f, FColor::Green, "100%");
    BatteryTestFunc(-1000.0f, FColor::Red, "0%");

    AddInfo("Battery charge / uncharge");

    Battery BatteryObject{0.6f};
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), .6f));
    BatteryObject.Uncharge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), .5f));
    BatteryObject.Charge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), .6f));

    AddInfo("Battery charge / uncharge. Curner cases");

    for (int32 i = 0; i < 100; ++i)
        BatteryObject.Uncharge();

    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.0f));

    for (int32 i = 0; i < 100; ++i)
        BatteryObject.Charge();

    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 1.0f));

    AddInfo("Battery comparison");

    const Battery BatteryLow{.1f};
    const Battery BatteryHigh{.8f};

    TestTrueExpr(BatteryHigh >= BatteryLow);
    TestTrueExpr(FMath::Max(BatteryHigh, BatteryLow) == BatteryHigh);

    AddInfo("Battery in memory comparison");

    TestNotSame("Not the same batteries", BatteryLow, BatteryHigh);

    const Battery BatteryHighDup{.8f};
    TestTrueExpr(BatteryHigh == BatteryHighDup);
    TestNotSame("Not the same batteries", BatteryHigh, BatteryHighDup);

    const Battery& BatteryHighRef = BatteryHigh;
    TestSame("The batteries is same in memory", BatteryHighRef, BatteryHigh);

    return true;
}

#endif