// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * * 包含原生 Gameplay Tags 的单例
 */
struct FAuraGameplayTags
{
public:
    //全局访问
    static const FAuraGameplayTags& Get() { return GameplayTags; }
    static void InitializeNativeGameplayTags();

protected:

private:
    static FAuraGameplayTags GameplayTags;
};