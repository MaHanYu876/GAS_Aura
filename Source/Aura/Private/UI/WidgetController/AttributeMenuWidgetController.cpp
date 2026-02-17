#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AttributeInfo.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
    // 目前为空，后续用于绑定属性改变时的回调
}

//该函数只有在广播初始值时才会触发（当初始化归属信息后初始HUD时在HUD的初始化函数中调用）
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
    UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

    check(AttributeInfo);

    FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
    Info.AttributeValue = AS->GetStrength();
    AttributeInfoDelegate.Broadcast(Info);
}