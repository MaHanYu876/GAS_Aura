#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AttributeInfo.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
    UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
    for (auto& Pair : AS->TagsToAttributes)
    {
        AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
            [this, Pair](const FOnAttributeChangeData& Data) -> void
            {
                BroadcastAttributeInfo(Pair.Key, Pair.Value());
            }
        );
    }
}

//该函数只有在广播初始值时才会触发（当初始化归属信息后初始HUD时在HUD的初始化函数中调用）
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
    UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

    check(AttributeInfo);

    for (auto& Pair : AS->TagsToAttributes)
    {
        BroadcastAttributeInfo(Pair.Key, Pair.Value());
        
    }
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
    //从Map中拿到当前 Tag，在 AttributeInfo 数据表里找到对应 UI 信息
    FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
    //Pair.Value()等价于： GetStrengthAttribute()返回的就是：FGameplayAttribute，你必须先提供这个，让系统知道你要获取哪个属性
    //然后再从 AttributeSet 里查值
    Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
    AttributeInfoDelegate.Broadcast(Info);
}