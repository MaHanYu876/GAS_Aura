// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputComponent.generated.h"

class UAuraInputConfig;
/**
 * 
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
    template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
    void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);

    // 函数的具体实现
    template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
    void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
    {
        // 确保传入的配置不为空，否则在调试时直接崩溃中断，方便定位问题
        check(InputConfig);

        // 遍历数据资产中配置的所有输入动作映射
        for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
        {
            // 只有当 InputAction 资源存在且关联的 GameplayTag 有效时才进行绑定
            if (Action.InputAction && Action.InputTag.IsValid())
            {
                // 如果传入了按下（Pressed）状态的处理函数
                if (PressedFunc)
                {
                    // 绑定到 Started 触发事件：当按键刚被按下时调用
                    // 注意最后一个参数 Action.InputTag，这是自定义组件的关键功能
                    BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
                }

                // 如果传入了松开（Released）状态的处理函数
                if (ReleasedFunc)
                {
                    // 绑定到 Completed 触发事件：当按键松开或动作完成时调用
                    BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
                }

                // 如果传入了按住（Held）状态的处理函数
                if (HeldFunc)
                {
                    // 绑定到 Triggered 触发事件：只要按键被按住，每帧都会调用
                    BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
                }
            }
        }
    }
};
