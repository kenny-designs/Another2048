// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlockGrid.generated.h"

UENUM()
enum class EBlockGridMoveDirection : uint8
{
	Left,
	Right,
	Up,
	Down
};

/** Class used to spawn blocks and manage score */
UCLASS(minimalapi)
class ABlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

public:
	ABlockGrid();

	void MoveGridBlocks(EBlockGridMoveDirection EDirection);

	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;

	/** Slots used to represent playfield **/
	UPROPERTY(Category=Grid, EditAnywhere)
	TSubclassOf<class ASlot> Slot;

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

private:
	/** Spawn block on to the Grid **/
	void SpawnBlock();

	/** Populate playfield with slots to visually form a grid */
	void SpawnAllGridSlots();

	/** The grid we are playing on **/
	TArray<class ABlock*> Grid;
};



