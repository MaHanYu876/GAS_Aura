// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	//用户控件实例指针
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
protected:

	virtual void BeginPlay() override;
private:
	//类类型限制器，在蓝图中指定用户控件类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
};
