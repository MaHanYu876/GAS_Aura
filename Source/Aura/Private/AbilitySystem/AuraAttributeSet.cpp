// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(10.f);
	InitMaxHealth(100.f);
	InitMana(10.f);
	InitMaxMana(50.f);

}

//注册同步规则
//REPNOTIFY_Always表示每次属性变化都会触发OnRep函数
void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}





//该函数当属性值被修改前调用，限制属性在正常范围内   
void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 限制生命值不超出 [0, MaxHealth]
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	// 限制法力值不超出 [0, MaxMana]
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

//如何对基础值进行修改的GE都会迪自动调用它，并根据相应的属性变化触发游戏事件，判断死亡等
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    Super::PostGameplayEffectExecute(Data);

    // 创建一个局部结构体变量
    FEffectProperties Props;

    // 将 Data 中的杂乱数据填入结构体中
    SetEffectProperties(Data, Props);

    // 3. 针对生命值（Health）进行限制处理
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        // 从内存读取当前刚写入的值，Clamp 后再次写回基础值
        SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
    }

    // 4. 针对法力值（Mana）进行限制处理
    if (Data.EvaluatedData.Attribute == GetManaAttribute())
    {
        // 同样确保法力值在 0 到最大值之间
        SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
    }
    // 后续你可以直接使用 Props.TargetCharacter 或 Props.SourceASC 进行逻辑判断
}


//从复杂的原始数据包（Data）中，把散乱的指针整理成易于阅读的结构体。
void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
    // Source = 发起者, Target = 受击者（即这个 AttributeSet 的拥有者）

    // 1. 提取上下文和源 ASC
    Props.EffectContextHandle = Data.EffectSpec.GetContext();
    Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

    // 2. 填充源（Source）信息
    if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
    {
        Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
        Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

        // 如果 Controller 为空但 Avatar 存在（比如 AI 角色），尝试从 Pawn 身上获取
        if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
        {
            if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
            {
                Props.SourceController = Pawn->GetController();
            }
        }

        if (Props.SourceController)
        {
            Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
        }
    }

    // 3. 填充目标（Target）信息
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
        Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
    }
}

