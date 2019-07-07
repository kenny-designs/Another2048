// Fill out your copyright notice in the Description page of Project Settings.


#include "BlockGrid.h"
#include "Block.h"
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

	SpawnBlock();

	// Loop to spawn each block
	/*
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(BlockLocation, FRotator(0,0,0));
	}
	*/
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

	// Add block to Grid
	Grid.Insert(NewBlock, BlockIndex);
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
