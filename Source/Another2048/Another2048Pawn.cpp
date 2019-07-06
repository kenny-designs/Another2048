// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Another2048Pawn.h"
#include "Block.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AAnother2048Pawn::AAnother2048Pawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AAnother2048Pawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Utilize VR headset if enabled
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			// TODO: Remove if support dropped for VR
		}
	}
}

void AAnother2048Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("OnResetVR", EInputEvent::IE_Pressed, this, &AAnother2048Pawn::OnResetVR);

	// Bind block movement input
	// TODO: This works but perhaps could be cleaned up. Refactor
	PlayerInputComponent->BindAction("ShiftBlocksLeft", EInputEvent::IE_Pressed, this, &AAnother2048Pawn::OnShiftBlocksLeft);
	PlayerInputComponent->BindAction("ShiftBlocksRight", EInputEvent::IE_Pressed, this, &AAnother2048Pawn::OnShiftBlocksRight);
	PlayerInputComponent->BindAction("ShiftBlocksUp", EInputEvent::IE_Pressed, this, &AAnother2048Pawn::OnShiftBlocksUp);
	PlayerInputComponent->BindAction("ShiftBlocksDown", EInputEvent::IE_Pressed, this, &AAnother2048Pawn::OnShiftBlocksDown);
}

void AAnother2048Pawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}

void AAnother2048Pawn::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAnother2048Pawn::OnShiftBlocksLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("shifted left"));
}

void AAnother2048Pawn::OnShiftBlocksRight()
{
	UE_LOG(LogTemp, Warning, TEXT("shifted right"));
}

void AAnother2048Pawn::OnShiftBlocksUp()
{
	UE_LOG(LogTemp, Warning, TEXT("shifted up"));
}

void AAnother2048Pawn::OnShiftBlocksDown()
{
	UE_LOG(LogTemp, Warning, TEXT("shifted down"));
}
