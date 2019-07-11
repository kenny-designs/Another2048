
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

/**
Manages the grid we are playing 2048 on. It's responsible for moving the blocks, combining them,
and checking if the game has been won or lost.

The game operates on a single Grid of type TArray<ABlock*> with length Size*Size.
Visually, the Grid takes on the form depicted by the ASCII art below where each number is the index
used to access that particular slot. (Assuming Size = 4)
____________________________
|      |      |      |      |
|  12  |  13  |  14  |  15  |
|______|______|______|______|
|      |      |      |      |
|   8  |   9  |  10  |  11  |
|______|______|______|______|
|      |      |      |      |
|   4  |   5  |   6  |   7  |
|______|______|______|______|
|      |      |      |      |
|   0  |   1  |   2  |   3  |
|______|______|______|______|

*/
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

	/** Slots used to represent playfield */
	UPROPERTY(Category=Grid, EditAnywhere)
	TSubclassOf<class ASlot> Slot;

	/** Returns DummyRoot subobject */
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

private:
	/** Spawn block on to the Grid */
	void SpawnBlockAtRandomLocation();

	/** Populate playfield with slots to visually form a grid */
	void SpawnAllGridSlots();

	/** Shift all blocks to one side of the playfield */
	void ShiftBlocksLeft();		// TODO: Currently runs on O(n^2) time. Change to O(n)
	void ShiftBlocksRight();	// TODO: Currently runs on O(n^2) time. Change to O(n)
	void ShiftBlocksUp();
	void ShiftBlocksDown();

	/** Updates the position of all Blocks to match the Grid TArray */
	void UpdateAllBlockPositions();

	/** Sets the PreviousGrid to be equal to the current Grid */
	void SetPreviousGridEqualToGrid();

	/** Returns true if the Grid is not equal to the PreviousGrid */
	bool bGridHasChanged();

	/** Returns an FVector with the world location of grid coordinate at specified index */
	FVector GetGridLocationAtIndex(int32 Index);

	/** Returns true if Grid TArray is full */
	bool bGridIsFull();

	/** The grid we are playing on */
	TArray<class ABlock*> Grid;

	/** Used for checking if the Grid has changed */
	TArray<class ABlock*> PreviousGrid;

	/** If no more moves can be made, game over! */
	bool bIsGameOver;
};



