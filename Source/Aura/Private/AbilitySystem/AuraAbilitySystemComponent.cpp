// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include <AuraGameplayTags.h>
#include <AbilitySystem/Abilities/AuraGameplayAbility.h>

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
	
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		// 1. 创建技能规范 (AbilitySpec)，为角色创建一个“1级 AbilityClass 技能的持有记录”。
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);

		// 2. 尝试将基类技能转换为我们自定义的 UAuraGameplayAbility
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			// 3. 将技能中定义的 StartupInputTag 添加到 Spec 的动态标签库中，建立技能与输入标签之间的关联，方便后续通过输入标签来触发技能。
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);

			// 4. 正式赋予角色该技能
			GiveAbility(AbilitySpec);
		}
	}
}

//在按键被按下或持续按住时触发。其核心逻辑是查找并激活技能。
void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	//使用 for 循环遍历 GetActivatableAbilities()，即角色目前身上所有被赋予且可以释放的技能实例（AbilitySpec）。
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		//查该技能是否匹配当前按下的按键标签（InputTag）。如果技能的 DynamicAbilityTags 中包含这个标签，说明这个技能与当前按键相关联。
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 标记输入已按下
			AbilitySpecInputPressed(AbilitySpec);
			// 如果技能未激活，尝试激活它
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 标记输入已松开
			AbilitySpecInputReleased(AbilitySpec);
		}
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
