// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Another2048GameMode.h"
#include "Another2048PlayerController.h"
#include "Another2048Pawn.h"

AAnother2048GameMode::AAnother2048GameMode()
{
	// no pawn by default
	DefaultPawnClass = AAnother2048Pawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AAnother2048PlayerController::StaticClass();
}
