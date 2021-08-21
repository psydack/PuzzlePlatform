// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "PlatformTrigger.h"

#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuSystem/MainMenu.h"

const static FName SESSION_NAME = TEXT("MySesionGame");

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuClassBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuClassBPClass.Class != nullptr)) return;

	MenuClass = MenuClassBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameBPClass.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetOnlineServiceName().ToString());

		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Is valid session"));

			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Found no subsystem"));
	}
}

void UPuzzlePlatformGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(InGameMenu != nullptr)) return;

	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::Host()
{
	if (SessionInterface.IsValid())
	{
		auto CurrentSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (CurrentSession != nullptr)
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful && SessionSearch.IsValid() && Menu != nullptr)
	{
		TArray<FString> ServerNames;
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionsComplete bSuccess: %d"), bWasSuccessful);

		UE_LOG(LogTemp, Warning, TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num());

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			ServerNames.Add(SearchResult.GetSessionIdStr());

			/*const auto Session = &SearchResult.Session;
			if (Session != NULL)
			{
				UE_LOG(LogTemp, Warning, TEXT("dumping Session: "));
				UE_LOG(LogTemp, Warning, TEXT("	OwningPlayerName: %s"), *Session->OwningUserName);
				UE_LOG(LogTemp, Warning, TEXT("	OwningPlayerId: %s"), Session->OwningUserId.IsValid() ? *Session->OwningUserId->ToDebugString() : TEXT(""));
				UE_LOG(LogTemp, Warning, TEXT("	NumOpenPrivateConnections: %d"), Session->NumOpenPrivateConnections);
				UE_LOG(LogTemp, Warning, TEXT("	NumOpenPublicConnections: %d"), Session->NumOpenPublicConnections);
				UE_LOG(LogTemp, Warning, TEXT("	SessionInfo: %s"), Session->SessionInfo.IsValid() ? *Session->SessionInfo->ToDebugString() : TEXT("NULL"));

				const auto SessionSettings = &Session->SessionSettings;
				if (SessionSettings != NULL)
				{
					UE_LOG(LogTemp, Warning, TEXT("dumping SessionSettings: "));
					UE_LOG(LogTemp, Warning, TEXT("\tNumPublicConnections: %d"), SessionSettings->NumPublicConnections);
					UE_LOG(LogTemp, Warning, TEXT("\tNumPrivateConnections: %d"), SessionSettings->NumPrivateConnections);
					UE_LOG(LogTemp, Warning, TEXT("\tbIsLanMatch: %s"), SessionSettings->bIsLANMatch ? TEXT("true") : TEXT("false"));
					UE_LOG(LogTemp, Warning, TEXT("\tbIsDedicated: %s"), SessionSettings->bIsDedicated ? TEXT("true") : TEXT("false"));
					UE_LOG(LogTemp, Warning, TEXT("\tbUsesStats: %s"), SessionSettings->bUsesStats ? TEXT("true") : TEXT("false"));
					UE_LOG(LogTemp, Warning, TEXT("\tbShouldAdvertise: %s"), SessionSettings->bShouldAdvertise ? TEXT("true") : TEXT("false"));
					UE_LOG(LogTemp, Warning, TEXT("\tbAllowJoinInProgress: %s"), SessionSettings->bAllowJoinInProgress ? TEXT("true") : TEXT("false"));
					UE_LOG(LogTemp, Warning, TEXT("\tbAllowInvites: %s"), SessionSettings->bAllowInvites ? TEXT("true") : TEXT("false"));
					UE_LOG(LogTemp, Warning, TEXT("\tbUsesPresence: %s"), SessionSettings->bUsesPresence ? TEXT("true") : TEXT("false"));
					UE_LOG(LogTemp, Warning, TEXT("\tbAllowJoinViaPresence: %s"), SessionSettings->bAllowJoinViaPresence ? TEXT("true") : TEXT("false"));
					UE_LOG(LogTemp, Warning, TEXT("\tbAllowJoinViaPresenceFriendsOnly: %s"), SessionSettings->bAllowJoinViaPresenceFriendsOnly ? TEXT("true") : TEXT("false"));
					UE_LOG(LogTemp, Warning, TEXT("\tBuildUniqueId: 0x%08x"), SessionSettings->BuildUniqueId);
					UE_LOG(LogTemp, Warning, TEXT("\tSettings:"));
				}
			}*/
		}
		Menu->SetServerList(ServerNames);
	}
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot create sesion."));
		return;
	}


	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
	}
}

void UPuzzlePlatformGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;

		UE_LOG(LogTemp, Warning, TEXT("Start find session"));

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformGameInstance::Join(const FString& Address)
{
	if (Menu != nullptr)
	{
		//Menu->SetServerList({ "Test" });
		/*Menu->Teardown();*/
	}

	/*UEngine* Engine = GetEngine();

	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);*/
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}