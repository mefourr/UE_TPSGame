// Copyright EpicGames.
using UnrealBuildTool;
using System.Collections.Generic;
public class TPS_GameServerTarget : TargetRules
{
    public TPS_GameServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V2;
        ExtraModuleNames.AddRange(new string[] { "TPS_Game" });
    }
}
