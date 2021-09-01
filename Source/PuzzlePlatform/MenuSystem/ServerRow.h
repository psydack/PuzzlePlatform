// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORM_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;

	UPROPERTY(meta = (BindWidget))
		class UButton* ServerButton;

	UPROPERTY(BlueprintReadOnly)
		bool bSelected = false;

	void Setup(class UMainMenu* AParent, uint32 AIndex);

private:

	UFUNCTION()
		void OnClicked();

	uint32 Index;
	class UMainMenu* Parent;
};
