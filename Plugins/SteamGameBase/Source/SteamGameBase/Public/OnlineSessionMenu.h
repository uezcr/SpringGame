// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionMenu.generated.h"

/**
 * 
 */
UCLASS()
class STEAMGAMEBASE_API UOnlineSessionMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberofPublicConnections = 4,FString TypeOfMatch = FString(TEXT("FreeForAll")));
protected:
	virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonOnClicked();
	UFUNCTION()
	void JoinButtonOnClicked();

	void MenuTearDown();

	//Subsystem
	class UOnlineSessionsSubsystem* OnlineSessionsSubsystem;

	int32 NumPublicConnections{4};
	FString MatchType{ TEXT("FreeForAll") };
};
