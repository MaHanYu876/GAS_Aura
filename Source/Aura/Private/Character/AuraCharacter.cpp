// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
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
