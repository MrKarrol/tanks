// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tanks : ModuleRules
{
	public Tanks(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "UMG", "MediaAssets", "DesktopPlatform", "QuestSystem" });

		PrivateDependencyModuleNames.AddRange(new string[] { "SlateCore", "Slate"});
		
		bEnableUndefinedIdentifierWarnings = false;

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
