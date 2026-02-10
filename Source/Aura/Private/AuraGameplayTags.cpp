#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;
//硬编码（Hardcode）**原生标签到引擎中，而不是仅仅依赖编辑器里的 .ini 文件。
void FAuraGameplayTags::InitializeNativeGameplayTags()
{
    UGameplayTagsManager::Get().AddNativeGameplayTag(
        FName("Attributes.Secondary.Armor"),
        FString("Reduces damage taken, improves Block Chance")
    );
}