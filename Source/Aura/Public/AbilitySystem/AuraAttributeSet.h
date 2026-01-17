// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

struct FGameplayAttributeData;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//定义gameplay属性
	//ReplicatedUsing = OnRep_Health 指定属性变化时调用的函数
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mona, Category = "Vital Attributes")
	FGameplayAttributeData Mona;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMona, Category = "Vital Attributes")
	FGameplayAttributeData MaxMona;

	//同步属性变化的函数（更新本地状态）
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mona(const FGameplayAttributeData& OldMona) const;

	UFUNCTION()
	void OnRep_MaxMona(const FGameplayAttributeData& OldMaxMona) const;

};
