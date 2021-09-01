// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"
#include "Components/Button.h"
#include "MainMenu.h"

void UServerRow::Setup(UMainMenu* AParent, uint32 AIndex)
{
	Parent = AParent;
	Index = AIndex;

	ServerButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
}

void UServerRow::OnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked on %d"), Index);
	if (!ensure(Parent != nullptr)) return;
	Parent->SelectIndex(Index);
}