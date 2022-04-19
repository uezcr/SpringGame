// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSessionMenu.h"
#include "Components/Button.h"
#include "OnlineSessionsSubsystem.h"

void UOnlineSessionMenu::MenuSetup()
{
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

void UOnlineSessionMenu::HostButtonOnClicked()
{
	if (OnlineSessionsSubsystem)
	{
		OnlineSessionsSubsystem->CreateSession(4, FString("FreeForAll"));
	}
}

void UOnlineSessionMenu::JoinButtonOnClicked()
{
}
