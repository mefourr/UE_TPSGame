#if WITH_AUTOMATION_TESTS

#include "TestUtils.h"

namespace TPS
{
namespace Test
{

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    if (!Object) return;

    FString Command = FString::Printf(TEXT("%s"), *FuncName);
    for (const auto& Param : Params)
    {
        Command.Append(" ").Append(Param);
    }

    FOutputDeviceNull OutputDeviceNull;
    Object->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
}

UWorld* GetTestGameWorld()
{
    const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
    for (const FWorldContext& Context : WorldContexts)
    {
        if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World())
        {
            return Context.World();
        }
    }
    return nullptr;
}

}  // namespace Test
}  // namespace TPS
#endif