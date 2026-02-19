// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include <AuraGameplayTags.h>

//该函数当确认ASC的归属信息后会被调用，通常在ASC初始化完成后，或者当ASC的拥有者发生变化时被调用。我们在这个函数中绑定了一个委托，当一个Gameplay Effect被应用到自己身上时，会触发EffectApplied函数。
void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("AbilityActorInfoSet: %s"), *GameplayTags.Attributes_Secondary_Armor.ToString()));
}

//接受一个技能数组，将其中的技能赋予角色
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		//首先会为角色创建一个“1级 AbilityClass 技能的持有记录”。
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		//GiveAbility(AbilitySpec);
		//GiveAbilityAndActivateOnce()函数将其注册到ASC并立即执行一次后移除，所以该函数适用于开局自动触发一次的能力
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
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
	EffectAssetTags.Broadcast(TagContainer);

	

	
}
