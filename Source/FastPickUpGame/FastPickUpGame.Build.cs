// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FastPickUpGame : ModuleRules
{
    public FastPickUpGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        // bLegacyPublicIncludePaths = true;  

        PublicDependencyModuleNames.AddRange(new[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "HeadMountedDisplay",
            "NetCore"
        });

        // PrivateDependencyModuleNames.AddRange(new[] { "Slate", "SlateCore" });
    }
}