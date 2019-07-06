// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Another2048Pawn.generated.h"

class ABlockGrid;

UCLASS(config=Game)
class AAnother2048Pawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	// Player input
	void OnShiftBlocksLeft();
	void OnShiftBlocksRight();
	void OnShiftBlocksUp();
	void OnShiftBlocksDown();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	ABlockGrid* BlockGrid = nullptr;

private:
	void FindBlockGridInScene();
};
