// Copyright EpicGames.
using UnrealBuildTool;
using System.Collections.Generic;
public class TPS_GameClientTarget : TargetRules
{
    public TPS_GameClientTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Client;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new string[] { "TPS_Game" });
    }
}
