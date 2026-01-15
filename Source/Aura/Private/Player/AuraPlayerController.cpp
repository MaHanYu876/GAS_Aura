// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h" // Include the header for UEnhancedInputLocalPlayerSubsystem

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
	check(Subsystem); // Ensure the subsystem is valid
	Subsystem->AddMappingContext(AuraContext, 0);

	// Mouse cursor settings
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Input mode settings
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); // Do not lock the mouse
	InputModeData.SetHideCursorDuringCapture(false); // Do not hide the cursor
	SetInputMode(InputModeData); // Apply the input mode
}
