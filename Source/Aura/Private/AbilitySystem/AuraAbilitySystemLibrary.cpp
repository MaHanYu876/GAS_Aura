// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"           // 用于获取 PlayerController
#include "UI/WidgetController/AuraWidgetController.h" // 用于 FWidgetControllerParams 结构体
#include "Player/AuraPlayerState.h"           // 用于获取 AbilitySystemComponent 和 AttributeSet
#include "UI/HUD/AuraHUD.h"                   // 用于访问 HUD 并获取 WidgetController

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
        {
            AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            UAttributeSet* AS = PS->GetAttributeSet();
            const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
            return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
        }
    }
    return nullptr;
}