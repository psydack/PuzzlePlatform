// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "ServerRow.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Kismet/KismetSystemLibrary.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelJoinMenuButton != nullptr)) return false;
	CancelJoinMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinServerButton != nullptr)) return false;
	JoinServerButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		MenuInterface->Host();
	}
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		UWorld* World = this->GetWorld();
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		if (!ensure(ServerList != nullptr)) return;
		ServerList->AddChild(Row);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{
	if (!ensure(QuitButton != nullptr)) return;

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	UKismetSystemLibrary::QuitGame(
		World,
		World->GetFirstLocalPlayerFromController<APlayerController>(),
		EQuitPreference::Quit,
		false);
}