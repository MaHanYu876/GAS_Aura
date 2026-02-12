// Copyright Droid Mechanics

#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
    check(GEngine); // 确保引擎实例存在

    UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);

    // 返回解引用的指针，确保外部拿到的是引用而不是指针
    return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
    Super::StartInitialLoading(); // 调用父类逻辑

    // 关键：在这里初始化我们的原生 Gameplay Tags
    FAuraGameplayTags::InitializeNativeGameplayTags();
}