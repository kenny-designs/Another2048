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

	// Do the same for the PreviousGrid
	PreviousGrid.Init(nullptr, NumBlocks);

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
	ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(Block, SpawnLocation, FRotator(0, 0, 0));

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
	// Create a TArray tracking the TopIndex for each column
	TArray<int32> TopColumnIndex;

	// Populate TopColumnIndex with the index for the top of each column in the Grid
	TopColumnIndex.Init(-1, Size);
	for (int32 Column = 0; Column < TopColumnIndex.Num(); ++Column)
	{
		TopColumnIndex[Column] = Grid.Num() - Size + Column;
	}

	// Iterate over Grid TArray from the last element of the second to top row to the first element in the array
	for (int32 Index = Grid.Num() - Size - 1; Index >= 0; --Index)
	{
		// Find the TopIndex of the Grid
		int32 Column = Index % Size;
		int32 TopIndex = TopColumnIndex[Column];

		// If we have no current block or if the TopIndex is less than 0, continue
		if (!Grid[Index] || TopIndex < 0) { continue; }

		// If there is no block at the TopIndex, swap
		if (!Grid[TopIndex])
		{
			Grid[TopIndex] = Grid[Index];
			Grid[Index] = nullptr;
		}
		// If both blocks are equal, double the top one, delete the other, and set a new TopColumnIndex for the column
		else if (*Grid[TopIndex] == *Grid[Index])
		{
			Grid[TopIndex]->DoubleBlockValue();
			Grid[Index]->Destroy();
			Grid[Index] = nullptr;
			TopColumnIndex[Column] = TopIndex - Size;
		}
		// If they are not equal, shift the current block up and set the new TopColumnIndex
		else
		{
			// Lower the TopIndex by a row
			TopIndex -= Size;
			TopColumnIndex[Column] = TopIndex;

			// Can't go any lower, continue
			if (TopIndex < 0) { continue; }	

			// Swap the CurrentBlock with the new TopIndex
			ABlock* CurrentBlock = Grid[Index];
			Grid[Index] = nullptr;
			Grid[TopIndex] = CurrentBlock;
		}
	}
}

void ABlockGrid::ShiftBlocksDown()
{
	// Create a TArray tracking the BottomIndex for each column
	TArray<int32> BottomColumnIndex;

	// Populate BottomColumnIndex with the index for the bottom of each column in the Grid
	BottomColumnIndex.Init(-1, Size);
	for (int32 Column = 0; Column < BottomColumnIndex.Num(); ++Column)
	{
		BottomColumnIndex[Column] = Column;
	}

	// Iterate over Grid TArray from the first element of the second row to the last element in the array
	for (int32 Index = Size; Index < Grid.Num(); ++Index)
	{
		// Find the BottomIndex of the Grid
		int32 Column = Index % Size;
		int32 BottomIndex = BottomColumnIndex[Column];

		// If we have no current block or if the BottomIndex is greater than or equal to Grid length, continue
		if (!Grid[Index] || BottomIndex >= Grid.Num()) { continue; }

		// If there is no block at the BottomIndex, swap
		if (!Grid[BottomIndex])
		{
			Grid[BottomIndex] = Grid[Index];
			Grid[Index] = nullptr;
		}
		// If both blocks are equal, double the top one, delete the other, and set a new BottomColumnIndex for the column
		else if (*Grid[BottomIndex] == *Grid[Index])
		{
			Grid[BottomIndex]->DoubleBlockValue();
			Grid[Index]->Destroy();
			Grid[Index] = nullptr;
			BottomColumnIndex[Column] = BottomIndex + Size;
		}
		// If they are not equal, shift the current block down and set the new BottomColumnIndex
		else
		{
			// Raise the BottomIndex by a row
			BottomIndex += Size;
			BottomColumnIndex[Column] = BottomIndex;

			// Can't go any higher, continue
			if (BottomIndex >= Grid.Num()) { continue; }	

			// Swap the CurrentBlock with the new BottomIndex
			ABlock* CurrentBlock = Grid[Index];
			Grid[Index] = nullptr;
			Grid[BottomIndex] = CurrentBlock;
		}
	}

}

void ABlockGrid::UpdateAllBlockPositions()
{
	for (int32 Index = 0; Index != Grid.Num(); ++Index)
	{
		if (Grid[Index])
		{
			// Move the block
			const FVector UpdatedLocation = GetGridLocationAtIndex(Index);
			Grid[Index]->MoveBlockToPosition(UpdatedLocation);
		}
	}
}

void ABlockGrid::SetPreviousGridEqualToGrid()
{
	for (int32 Index = 0; Index < Grid.Num(); ++Index)
	{
		PreviousGrid[Index] = Grid[Index];
	}
}

bool ABlockGrid::bGridHasChanged()
{
	for (int32 Index = 0; Index < Grid.Num(); ++Index)
	{
		if (PreviousGrid[Index] != Grid[Index])
		{ 
			return true;
		}
	}

	return false;
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

void ABlockGrid::MoveGridBlocks(EBlockGridMoveDirection EDirection)
{
	// Return if no more moves can be made
	if (bIsGameOver) { return; }

	// Log the current state of the Grid before moving anything
	SetPreviousGridEqualToGrid();

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

	// If the Grid hasn't changed, return
	if (!bGridHasChanged()) { return; }

	// Otherwise, update all blocks world positions
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
