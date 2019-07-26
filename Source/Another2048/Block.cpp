// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Components/TextRenderComponent.h"
#include "Scoreboard.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "PuzzleBlock"

// Initialize static member variables
AScoreboard* ABlock::Scoreboard = nullptr;

ABlock::ABlock()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create text render component
	BlockValueLabel = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BlockValue0"));
	BlockValueLabel->SetRelativeRotation(FRotator(90.f, 180.f, 0.f));
	BlockValueLabel->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
	BlockValueLabel->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	BlockValueLabel->SetVerticalAlignment(EVerticalTextAligment::EVRTA_TextCenter);
	BlockValueLabel->SetTextRenderColor(FColor::Black);
	BlockValueLabel->SetXScale(4.f);
	BlockValueLabel->SetYScale(4.f);
	BlockValueLabel->SetText(FText::Format(LOCTEXT("BlockValueFmt", "{0}"), FText::AsNumber(BlockValue)));
	BlockValueLabel->SetupAttachment(DummyRoot);
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();

	// Find the scoreboard within the scene
	if (!ABlock::Scoreboard)
	{
		TArray<AActor*> ScoreboardFound;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScoreboard::StaticClass(), ScoreboardFound);

		// If too few/many Scoreboards in the scene, return and give a warning
		if (!ensureMsgf(!(ScoreboardFound.Num() < 1), TEXT("No Scoreboard found in the scene!")) ||
			!ensureMsgf(!(ScoreboardFound.Num() > 1), TEXT("Too many Scoreboards found in the scene!"))) { return; }

		// Otherwise, get pointer to first Scoreboard found
		ABlock::Scoreboard = Cast<AScoreboard>(ScoreboardFound[0]);
	}
}

void ABlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::EndPlayInEditor &&
		ABlock::Scoreboard)
	{
		ABlock::Scoreboard = nullptr;
	}
}

int32 ABlock::GetBlockValue() const
{
	return BlockValue;
}

void ABlock::DoubleBlockValue()
{
	BlockValue *= 2;
	BlockValueLabel->SetText(FText::Format(LOCTEXT("BlockValueFmt", "{0}"), FText::AsNumber(BlockValue)));

	// Add new BlockValue to the Scoreboard
	Scoreboard->AddToScore(BlockValue);
}

void ABlock::PlayRandomDeathSound() const
{
	int32 Index = FMath::RandHelper(DestructionSoundWaves.Num());
	UGameplayStatics::PlaySound2D(GetWorld(), DestructionSoundWaves[Index]);
}

#undef LOCTEXT_NAMESPACE
