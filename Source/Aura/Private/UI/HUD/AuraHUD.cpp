// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

//创建控件控制器，并设置相应的参数
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr) //
	{
		// 实例化控制器对象
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass); 
		// 投递包裹
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
	}
	return OverlayWidgetController; //
}

//创建控件，设置控件的控制器，并把控件添加到屏幕上
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 安全检查
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class 未初始化"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class 未初始化"));

	// 1. 创建 Widget 实例
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget); 

	// 2. 打包参数对象
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

	// 3. 获取并设置控制器
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	OverlayWidget->SetWidgetController(WidgetController); //

	//广播初始值
	WidgetController->BroadcastInitialValues(); 

	// 4. 显示到屏幕
	Widget->AddToViewport();
}

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}

