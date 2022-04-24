// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSessionMenu.h"
#include "Components/Button.h"
#include "OnlineSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"


void UOnlineSessionMenu::MenuSetup(int32 NumberofPublicConnections, FString TypeOfMatch, FString LobbyPath)
{
	PathToLobby = FString::Printf(TEXT("%s?listen"), *LobbyPath);
	NumPublicConnections = NumberofPublicConnections;
	MatchType = TypeOfMatch;
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		OnlineSessionsSubsystem = GameInstance->GetSubsystem<UOnlineSessionsSubsystem>();
	}
	if (OnlineSessionsSubsystem)
	{
		OnlineSessionsSubsystem->SpringOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSessionComplete);
		OnlineSessionsSubsystem->SpringOnFindSessionComplete.AddUObject(this, &ThisClass::OnFindSessionComplete);
		OnlineSessionsSubsystem->SpringOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSessionComplete);
		OnlineSessionsSubsystem->SpringOnDestroySessionComplete.AddDynamic(this, &ThisClass::OnDestroySessionComplete);
		OnlineSessionsSubsystem->SpringOnStartSessionComplete.AddDynamic(this, &ThisClass::OnStartSessionComplete);
	}
}

bool UOnlineSessionMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonOnClicked);
	}
	if (JoinButton)
	{
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonOnClicked);
	}
	return true;
}

void UOnlineSessionMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();
	Super::OnLevelRemovedFromWorld(InLevel,InWorld);
}

void UOnlineSessionMenu::OnCreateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Blue,
				FString(TEXT("Session Create Failed!"))
			);
			HostButton->SetIsEnabled(true);
		}
	}
}

void UOnlineSessionMenu::OnFindSessionComplete(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (OnlineSessionsSubsystem == nullptr)
	{
		return;
	}
	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue);
		if (SettingsValue == MatchType)
		{
			OnlineSessionsSubsystem->JoinSessiion(Result);
			return;
		}
	}
	if (!bWasSuccessful||SessionResults.Num()==0)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UOnlineSessionMenu::OnJoinSessionComplete(EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		IOnlineSessionPtr SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			FString Address;
			SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}

	}
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		JoinButton->SetIsEnabled(true);
	}
}

void UOnlineSessionMenu::OnDestroySessionComplete(bool bWasSuccessful)
{
}

void UOnlineSessionMenu::OnStartSessionComplete(bool bWasSuccessful)
{
}

void UOnlineSessionMenu::HostButtonOnClicked()
{
	HostButton->SetIsEnabled(false);
	if (OnlineSessionsSubsystem)
	{
		OnlineSessionsSubsystem->CreateSession(NumPublicConnections, MatchType);
	}
}

void UOnlineSessionMenu::JoinButtonOnClicked()
{
	JoinButton->SetIsEnabled(false);
	if (OnlineSessionsSubsystem)
	{
		OnlineSessionsSubsystem->FindSessions(10000);
	}
}

void UOnlineSessionMenu::MenuTearDown()
{
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}

}
