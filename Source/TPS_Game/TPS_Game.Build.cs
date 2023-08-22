// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPS_Game : ModuleRules
{
    public TPS_Game(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
    }
}
