// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
//创建控件控制器，并设置相应的参数
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr) //
	{
		// 实例化控制器对象
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass); 
		// 投递包裹
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		//当widgetcontroller成功设置好属性集后，就可以订阅gas委托系统，来监听属性的变化
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController; //
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		// 如果控制器还没被创建，则根据指定的类创建一个新对象
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);

		// 设置控制器所需的参数（如 PlayerState, AbilitySystemComponent 等）
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);

		// 绑定回调到数据源，以便属性变化时 UI 能自动更新
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
	
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

