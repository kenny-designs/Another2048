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
	// TODO: Make this into a member variable?
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

	// Spawn a block
	const FVector SpawnLocation = GetGridLocationAtIndex(BlockIndex);
	ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(SpawnLocation, FRotator(0, 0, 0));

	// Add block to Grid TArray
	Grid[BlockIndex] = NewBlock;
}

void ABlockGrid::SpawnAllGridSlots()
{
	const int32 TotalSlots = Size * Size;

	// Loop to spawn each slot
	for(int32 SlotIndex=0; SlotIndex < TotalSlots; SlotIndex++)
	{
		// Spawn grid slot
		const FVector SpawnLocation = GetGridLocationAtIndex(SlotIndex);
		GetWorld()->SpawnActor<ASlot>(Slot, SpawnLocation, FRotator(0, 0, 0));
	}
}

void ABlockGrid::ShiftBlocksLeft()
{
	// Make sure we don't merge in the same spot twice
	int32 LastMergedIndex = -1;

	// Loop through the entire Grid
	for (int32 Index = 0; Index < Grid.Num(); ++Index)
	{
		// If we encounter a block, shift it as far as it can go to the left
		if (Grid[Index])
		{
			// Find left side index
			int32 LeftIndex = Index - (Index % Size);
			for (; LeftIndex < Index; ++LeftIndex)
			{
				// If empty, swap
				if (!Grid[LeftIndex])
				{
					Grid[LeftIndex] = Grid[Index];
					Grid[Index] = nullptr;
					break;
				}
				// Else if blocks share the same value and we haven't merged at that spot yet, double and delete one
				else if (*Grid[LeftIndex] == *Grid[Index] &&
						 LastMergedIndex != LeftIndex)
				{
					Grid[LeftIndex]->DoubleBlockValue();
					Grid[Index]->Destroy();
					Grid[Index] = nullptr;
					LastMergedIndex = LeftIndex;
					break;
				}
			}
		}
	}
}

void ABlockGrid::ShiftBlocksRight()
{
	// Make sure we don't merge in the same spot twice
	int32 LastMergedIndex = -1;

	// Loop through the entire Grid
	for (int32 Index = Grid.Num()-1; Index >= 0; --Index)
	{
		// If we encounter a block, shift it as far as it can go to the right
		if (Grid[Index])
		{
			// Find right side index
			// TODO: Can this be simplified?
			int32 RightIndex = Index + Size - 1 - Index % Size;
			for (; RightIndex > Index; --RightIndex)
			{
				// If empty, swap
				if (!Grid[RightIndex])
				{
					Grid[RightIndex] = Grid[Index];
					Grid[Index] = nullptr;
					break;
				}
				// Else if blocks share the same value and we haven't merged at that spot yet, double and delete one
				else if (*Grid[RightIndex] == *Grid[Index] &&
						 LastMergedIndex != RightIndex)
				{
					Grid[RightIndex]->DoubleBlockValue();
					Grid[Index]->Destroy();
					Grid[Index] = nullptr;
					LastMergedIndex = RightIndex;
					break;
				}
			}
		}
	}
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
			// Move the block
			const FVector UpdatedLocation = GetGridLocationAtIndex(Index);
			Grid[Index]->SetActorLocation(UpdatedLocation);
		}
	}
}

FVector ABlockGrid::GetGridLocationAtIndex(int32 Index)
{
	const float XOffset = (Index / Size) * BlockSpacing; // Divide by dimension
	const float YOffset = (Index % Size) * BlockSpacing; // Modulo gives remainder

	return FVector(XOffset, YOffset, 0.f) + GetActorLocation();
}

// TODO: Could be simplifed using a method from TArray
bool ABlockGrid::bGridIsFull()
{
	for (ABlock* Block : Grid)
	{
		if (Block == nullptr) { return false; }
	}
	return true;
}

// TODO: Implement actual grid movement
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

	// TODO: Only do the following if we shifted successfully
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
