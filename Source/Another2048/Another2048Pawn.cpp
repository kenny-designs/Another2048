// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Another2048Pawn.h"
#include "BlockGrid.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AAnother2048Pawn::AAnother2048Pawn(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AAnother2048Pawn::BeginPlay()
{
	Super::BeginPlay();

	FindBlockGridInScene();
}

void AAnother2048Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind block movement input
	// TODO: This works but perhaps could be cleaned up. Refactor
	PlayerInputComponent->BindAction("ShiftBlocksLeft", EInputEvent::IE_Pressed, this, &AAnother2048Pawn::OnShiftBlocksLeft);
	PlayerInputComponent->BindAction("ShiftBlocksRight", EInputEvent::IE_Pressed, this, &AAnother2048Pawn::OnShiftBlocksRight);
	PlayerInputComponent->BindAction("ShiftBlocksUp", EInputEvent::IE_Pressed, this, &AAnother2048Pawn::OnShiftBlocksUp);
	PlayerInputComponent->BindAction("ShiftBlocksDown", EInputEvent::IE_Pressed, this, &AAnother2048Pawn::OnShiftBlocksDown);
}

void AAnother2048Pawn::OnShiftBlocksLeft()  { BlockGrid->MoveGridBlocks(EBlockGridMoveDirection::Left); }
void AAnother2048Pawn::OnShiftBlocksRight() { BlockGrid->MoveGridBlocks(EBlockGridMoveDirection::Right); }
void AAnother2048Pawn::OnShiftBlocksUp()    { BlockGrid->MoveGridBlocks(EBlockGridMoveDirection::Up); }
void AAnother2048Pawn::OnShiftBlocksDown()  { BlockGrid->MoveGridBlocks(EBlockGridMoveDirection::Down); }

void AAnother2048Pawn::FindBlockGridInScene()
{
	TArray<AActor*> FoundBlockGrid;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABlockGrid::StaticClass(), FoundBlockGrid);
	BlockGrid = Cast<ABlockGrid>(FoundBlockGrid.Pop());
}
