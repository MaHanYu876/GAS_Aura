// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	//打印GE类的名字
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Effect Applied: %s"), *EffectSpec.Def->GetName()));

	//========打印GE类每一个标签名字
	// 1. 创建一个标签容器来接收数据
	FGameplayTagContainer TagContainer;

	// 2. 从 EffectSpec 中获取所有的 Asset Tags (在 GE 资产面板配置的 Tags)
	EffectSpec.GetAllAssetTags(TagContainer);

	// 3. 遍历容器中的每一个标签
	for (const FGameplayTag& Tag : TagContainer)
	{
		// TODO: 之后这里会将 Tag 广播给 Widget Controller 以显示 UI 图标

		// 格式化调试消息
		const FString Msg = FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString());

		// 在屏幕上显示蓝色文字，持续 8 秒
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
		}
	}
}
