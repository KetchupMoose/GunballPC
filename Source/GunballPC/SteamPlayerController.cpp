// Fill out your copyright notice in the Description page of Project Settings.

#include "GunballPC.h"
#include "OnlineSubsystem.h"
#include "SteamPlayerController.h"

FString ASteamPlayerController::GetSessionTicket()
{
	FString SessionTicket = OnlineInterface->GetIdentityInterface()->GetAuthToken(0);
	return SessionTicket;

}


