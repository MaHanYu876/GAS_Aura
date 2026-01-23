// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h" // Include the header for UEnhancedInputLocalPlayerSubsystem
#include <EnhancedInputComponent.h>
#include "Interaction/EnemyInterface.h"


AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; // Mark this actor to replicate between server and client
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext); // Ensure the mapping context is assigned in the editor; crashes if not assigned in the blueprint

	// Retrieve the Enhanced Input Subsystem for the local player
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)// Ensure the subsystem is valid
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	
	

	// Mouse cursor settings
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Input mode settings
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Do not lock the mouse
	InputModeData.SetHideCursorDuringCapture(false); // Do not hide the cursor
	SetInputMode(InputModeData); // Apply the input mode
}
void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	}
}
void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	if (APawn* ControlledPawn = GetPawn<APawn>()) 
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

//该函数用于每帧检测鼠标下的对象，并根据检测结果更新敌人高亮状态
void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	// 1. 获取鼠标下的射线检测结果，使用 Visibility 通道
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;//如果没有击中任何对象，直接返回
	
	// 2. 更新状态：保存上一帧的 Actor，并获取当前帧击中的敌人接口
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/**
	 * 逻辑分支处理（5种场景）：
	 * A. 上一帧为空 && 当前帧为空 -> 不做任何操作
	 * B. 上一帧为空 && 当前帧有效 -> 高亮当前目标
	 * C. 上一帧有效 && 当前帧为空 -> 取消高亮上个目标
	 * D. 两者均有效但不同 -> 取消旧目标高亮，开启新目标高亮
	 * E. 两者均有效且相同 -> 不做任何操作
	 */

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// 情况 B
			ThisActor->HighLightEnemy();
		}
		else
		{
			// 情况 A - 两者均为空，不做处理
		}
	}
	else // LastActor 有效
	{
		if (ThisActor == nullptr)
		{
			// 情况 C
			LastActor->UnHighLightEnemy();
		}
		else // 两者均有效
		{
			if (LastActor != ThisActor)
			{
				// 情况 D
				LastActor->UnHighLightEnemy();
				ThisActor->HighLightEnemy();
			}
			else
			{
				// 情况 E - 停留在同一个目标上，不做处理
			}
		}
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}