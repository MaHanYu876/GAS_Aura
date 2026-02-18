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

    for (auto& Pair : AS->TagsToAttributes)
    {
        //从Map中拿到当前 Tag，在 AttributeInfo 数据表里找到对应 UI 信息
        FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
        //Pair.Value()等价于： GetStrengthAttribute()返回的就是：FGameplayAttribute，你必须先提供这个，让系统知道你要获取哪个属性
        //然后再从 AttributeSet 里查值
        Info.AttributeValue = Pair.Value().GetNumericValue(AS);
        AttributeInfoDelegate.Broadcast(Info);
    }
}