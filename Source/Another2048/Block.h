// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class ABlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

	/** Text component for the score */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* BlockValueLabel;

public:
	ABlock();

	int32 GetBlockValue() const;

	void DoubleBlockValue();

	/** Smoothly translates the Block to the given EndPosition */
	UFUNCTION(BlueprintImplementableEvent, Category = "Movement")
	void MoveBlockToPosition(FVector EndPosition);

	/** Returns true if two ABlocks have the same BlockValue  */
	FORCEINLINE bool operator==(const ABlock& OtherBlock) const
	{
		return BlockValue == OtherBlock.BlockValue;
	}

	/** Basic white material for the block **/
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }

private:
	/** The block's current value. Defaults to 2 **/
	int32 BlockValue = 2;
};
