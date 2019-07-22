// Fill out your copyright notice in the Description page of Project Settings.


#include "Block.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/TextRenderComponent.h"
#include "Materials/MaterialInstance.h"

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

int32 ABlock::GetBlockValue() const
{
	return BlockValue;
}

void ABlock::DoubleBlockValue()
{
	BlockValue *= 2;
	BlockValueLabel->SetText(FText::Format(LOCTEXT("BlockValueFmt", "{0}"), FText::AsNumber(BlockValue)));
}

#undef LOCTEXT_NAMESPACE
