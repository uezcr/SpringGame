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
	void MenuSetup();
protected:
	virtual bool Initialize() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostButtonOnClicked();
	UFUNCTION()
	void JoinButtonOnClicked();

	//Subsystem
	class UOnlineSessionsSubsystem* OnlineSessionsSubsystem;
};
