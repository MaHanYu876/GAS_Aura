// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Aura : ModuleRules
{
	public Aura(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		PrivateDependencyModuleNames.AddRange(new string[] {
			"GameplayAbilities", // GAS 核心逻辑
			"GameplayTags",      // GAS 状态标签
			"GameplayTasks"      // 解决 LNK2001 报错的关键模块 });

			// Uncomment if you are using Slate UI
			// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

			// Uncomment if you are using online features
			// PrivateDependencyModuleNames.Add("OnlineSubsystem");

			// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		});
	}
}
