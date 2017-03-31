// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "GunballGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class GUNBALLPC_API AGunballGameMode : public AGameMode
{
	GENERATED_BODY()

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void PostSeamlessTravel() override;


public:
	// Sets default values for this character's properties
	//AWallBallGameModeBase();

	//make player1 score property
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Variables")
		int player1Score = 0;

	//make player2 score property
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Variables")
		int player2Score = 0;

	//playerWinProperty
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Variables")
		int playerWin = 0;

	//Calculate Game State
	virtual void CalculateGame();

	//Calculate Score Function
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		virtual void CalculateScore(int PlayerGoal);

	//blueprint impelmentable event for handling server reload
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Server Travel")
	void ServerTravelled();

};
