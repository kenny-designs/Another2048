// Fill out your copyright notice in the Description page of Project Settings.

#include "BlockGrid.h"
#include "Block.h"
#include "Slot.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"

ABlockGrid::ABlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Set defaults
	Size = 4;
	BlockSpacing = 300.f;
}

void ABlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	// Create an empty grid of length NumBlocks
	Grid.Init(nullptr, NumBlocks);

	// Setup playfield
	SpawnBlock();
	SpawnAllGridSlots();
}

void ABlockGrid::SpawnBlock()
{
	// TODO: remove. make more dynamic
	int32 BlockIndex = 3; // spawning at (3,0)

	const float XOffset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
	const float YOffset = (BlockIndex % Size) * BlockSpacing; // Modulo gives remainder

	// Make position vector, offset from Grid location
	const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

	// Spawn a block
	ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(BlockLocation, FRotator(0, 0, 0));

	// Add block to Grid TArray
	Grid.Insert(NewBlock, BlockIndex);
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

// TODO: Implement actual grid movement
void ABlockGrid::MoveGridBlocks(EBlockGridMoveDirection EDirection)
{
	switch (EDirection)
	{
	case EBlockGridMoveDirection::Left:
		UE_LOG(LogTemp, Warning, TEXT("lefts"));
		break;
	case EBlockGridMoveDirection::Right:
		UE_LOG(LogTemp, Warning, TEXT("rights"));
		break;
	case EBlockGridMoveDirection::Up:
		UE_LOG(LogTemp, Warning, TEXT("ups"));
		break;
	case EBlockGridMoveDirection::Down:
		UE_LOG(LogTemp, Warning, TEXT("downs"));
		break;
	}
}
