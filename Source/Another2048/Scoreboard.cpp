// Fill out your copyright notice in the Description page of Project Settings.


#include "Scoreboard.h"
#include "Components/TextRenderComponent.h"

#define LOCTEXT_NAMESPACE "PuzzleScoreboard"

// Sets default values
AScoreboard::AScoreboard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create text render component
	ScoreboardLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreboardValue"));
	ScoreboardLabel->SetXScale(4.f);
	ScoreboardLabel->SetYScale(4.f);
	ScoreboardLabel->SetText(FText::Format(LOCTEXT("ScoreboardFmt", "{0}"), FText::AsNumber(Score)));
	ScoreboardLabel->SetupAttachment(DummyRoot);
}

void AScoreboard::AddToScore(int32 ValueToAdd)
{
	Score += ValueToAdd;
	ScoreboardLabel->SetText(FText::Format(LOCTEXT("ScoreboardFmt", "{0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE
