// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Another2048Pawn.generated.h"

UCLASS(config=Game)
class AAnother2048Pawn : public APawn
{
	GENERATED_UCLASS_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

protected:
	void OnResetVR();

	// Player input
	void OnShiftBlocksLeft();
	void OnShiftBlocksRight();
	void OnShiftBlocksUp();
	void OnShiftBlocksDown();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ABlock* CurrentBlockFocus;
};
