// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"

#include "MainMenu.generated.h"

/**
 *
 */
UCLASS()
class PUZZLEPLATFORM_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);
	bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget));
	class UButton* Host;

	UPROPERTY(meta = (BindWidget));
	class UButton* Join;

	UFUNCTION()
	void HostServer();

	IMenuInterface* MenuInterface;
};
