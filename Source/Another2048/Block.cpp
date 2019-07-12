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
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BaseMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);

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

	// Save a pointer to the BaseMaterial
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
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
