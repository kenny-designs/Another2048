// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockGrid.h"
#include "Block.h"
#include "Slot.h"
#include "Engine/World.h"

ABlockGrid::ABlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Set defaults
	Size = 4;
	BlockSpacing = 300.f;
	bIsGameOver = false;
}

void ABlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	// Create an empty grid of length NumBlocks
	Grid.Init(nullptr, NumBlocks);

	// Setup playfield
	SpawnBlockAtRandomLocation();
	SpawnAllGridSlots();
}

void ABlockGrid::SpawnBlockAtRandomLocation()
{
	// Find a suitable spot to spawn the block
	int32 BlockIndex = 0;
	do
	{
		BlockIndex = FMath::RandHelper(Size*Size);
	} while (Grid[BlockIndex]);

	const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
	const float YOffset = (BlockIndex % Size) * BlockSpacing; // Modulo gives remainder

	// Make position vector, offset from Grid location
	const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

	// Spawn a block
	ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(BlockLocation, FRotator(0, 0, 0));

	// Add block to Grid TArray
	Grid[BlockIndex] = NewBlock;
}

void ABlockGrid::SpawnAllGridSlots()
{
	const int32 TotalSlots = Size * Size;

	// Loop to spawn each slot
	for(int32 SlotIndex=0; SlotIndex < TotalSlots; SlotIndex++)
	{
		const float XOffset = (SlotIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (SlotIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector SpawnLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn grid slot
		GetWorld()->SpawnActor<ASlot>(Slot, SpawnLocation, FRotator(0, 0, 0));
	}
}

void ABlockGrid::ShiftBlocksLeft()
{
	UE_LOG(LogTemp, Warning, TEXT("lefts"));
	for (int32 Index = 1; Index < Grid.Num(); ++Index)
	{
		if (Grid[Index] && !Grid[Index-1])
		{
			Grid[Index - 1] = Grid[Index];
			Grid[Index] = nullptr;
		}
	}
}

void ABlockGrid::ShiftBlocksRight()
{
	UE_LOG(LogTemp, Warning, TEXT("rights"));
}

void ABlockGrid::ShiftBlocksUp()
{
	UE_LOG(LogTemp, Warning, TEXT("ups"));
}

void ABlockGrid::ShiftBlocksDown()
{
	UE_LOG(LogTemp, Warning, TEXT("downs"));
}

void ABlockGrid::UpdateAllBlockPositions()
{
	for (int32 Index = 0; Index != Grid.Num(); ++Index)
	{
		if (Grid[Index])
		{
			const float XOffset = (Index / Size) * BlockSpacing; // Divide by dimension
			const float YOffset = (Index % Size) * BlockSpacing; // Modulo gives remainder

			// Make position vector, offset from Grid location
			const FVector UpdatedLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

			Grid[Index]->SetActorLocation(UpdatedLocation);
		}
	}
}

bool ABlockGrid::bGridIsFull()
{
	for (ABlock* Block : Grid)
	{
		if (Block == nullptr) { return false; }
	}
	return true;
}

// TODO: Implement actual grid movement
// TODO: Remove UE_LOGs
void ABlockGrid::MoveGridBlocks(EBlockGridMoveDirection EDirection)
{
	// Return if no more moves can be made
	if (bIsGameOver) { return; }

	// Move blocks
	switch (EDirection)
	{
	case EBlockGridMoveDirection::Left:
		ShiftBlocksLeft();
		break;
	case EBlockGridMoveDirection::Right:
		ShiftBlocksRight();
		break;
	case EBlockGridMoveDirection::Up:
		ShiftBlocksUp();
		break;
	case EBlockGridMoveDirection::Down:
		ShiftBlocksDown();
		break;
	}

	// Update all blocks world positions
	UpdateAllBlockPositions();

	// Spawn new block if able
	if (!bGridIsFull())
	{
		SpawnBlockAtRandomLocation();
	}
	// Otherwise, game over
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("G A M E O V E R ! ! !"));
		bIsGameOver = true;
	}
}
