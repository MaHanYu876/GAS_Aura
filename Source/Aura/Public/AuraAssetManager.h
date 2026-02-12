// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
    // 单例访问点
    static UAuraAssetManager& Get();

protected:
    // 引擎启动时初始化的核心钩子函数
    virtual void StartInitialLoading() override;
};
