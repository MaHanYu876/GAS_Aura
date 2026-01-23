// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Player/AuraPlayerState.h>
#include "AbilitySystemComponent.h"
#include <Player/AuraPlayerController.h>
#include <UI/HUD/AuraHUD.h>
AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;//角色朝向自动旋转到移动方向
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);//角色旋转速率
	GetCharacterMovement()->bConstrainToPlane = true;//角色运动约束在一个平面上
	GetCharacterMovement()->bSnapToPlaneAtStart = true;//角色开始时贴合平面

	//控制器的旋转不会带动角色模型旋转
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
	
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	//初始化HUD，只有当玩家控制器取得了ASC和AS的归属权之后，此时才能初始化HUD，传递各种参数
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			// 调用 HUD 的初始化函数，传入核心 GAS 组件引用
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

}
