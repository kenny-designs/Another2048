// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"

#define LOCTEXT_NAMESPACE "PuzzleBlock"

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
	if (!Scoreboard)
	{
		TArray<AActor*> ScoreboardFound;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScoreboard::StaticClass(), ScoreboardFound);

		// If we didn't find anything, return
		// TODO: Add warning if nothing was found
		// TODO: Also, make it so we aren't constantly looking for the Scoreboard
		if (ScoreboardFound.Num() < 1) { return; }

		// Otherwise, get pointer to first Scoreboard found
		Scoreboard = Cast<AScoreboard>(ScoreboardFound[0]);
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
