// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"

struct FGameplayTag;
//定义了一个结构体，用于将“输入操作”与“Gameplay标签”绑定，并创建了一个存储这些绑定的数据资产类。
USTRUCT(BlueprintType)
struct FAuraInputAction
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    const class UInputAction* InputAction = nullptr;

    UPROPERTY(EditDefaultsOnly)
    FGameplayTag InputTag = FGameplayTag();
};
/**
 * 
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:
    // 根据标签查找对应的输入操作
    const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TArray<FAuraInputAction> AbilityInputActions;
};
