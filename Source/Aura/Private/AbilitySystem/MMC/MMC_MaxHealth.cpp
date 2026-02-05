// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MMC/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
    // 1. 设置要捕获的属性：Vigor（活力）
    VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
    // 2. 确定来源：Target（通常 MaxHealth 计算是基于目标自身的 Vigor）
    VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    // 3. 是否快照：False（实时获取最新值，而不是 Effect 创建时的值）
    VigorDef.bSnapshot = false;

    // 将该定义添加到捕获列表中
    RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    // A. 收集 Source 和 Target 的标签
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    // B. 设置评估参数
    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    // C. 获取捕获到的 Vigor 属性值
    float Vigor = 0.f;
    GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
    Vigor = FMath::Max<float>(Vigor, 0.f); // 确保属性不为负数

    // D. 通过接口获取角色等级（Level）
    // 通常从 EffectContext 的 SourceObject 中获取实现了接口的 Actor
    ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
    const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

    // E. 最终计算公式：基础 80 + 2.5倍活力 + 10倍等级
    return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}