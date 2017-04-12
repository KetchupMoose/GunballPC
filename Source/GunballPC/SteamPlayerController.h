// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "CoreOnline.h"
#include "Online.h"
#include "SteamPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GUNBALLPC_API ASteamPlayerController : public APlayerController
{
	GENERATED_BODY()


		
		IOnlineSubsystem* OnlineInterface = IOnlineSubsystem::Get();
	
		UFUNCTION(BlueprintCallable, Category = "Steam")
		FString GetSessionTicket();

};
