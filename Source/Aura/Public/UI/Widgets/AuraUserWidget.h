// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	//持有WidgetController的引用，不直接访问GAS的数据，而是通过Controller间接获取
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
protected:
	//当c++层通过SetWidgetController设置了WidgetController后，会调用这个事件通知蓝图层绑定数据到UI
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();

};
