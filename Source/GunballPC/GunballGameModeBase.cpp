// Fill out your copyright notice in the Description page of Project Settings.

#include "GunballPC.h"
#include "GunballGameModeBase.h"


// Called when the game starts or when spawned
void AGunballGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGunballGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//implement CalculateScore
void AGunballGameModeBase::CalculateScore(int PlayerGoal)
{
	if (PlayerGoal == 1)
	{
		player2Score += 1;
	}
	else
	{
		player1Score += 1;
	}
	CalculateGame();

}

//implement CalculateGame
void AGunballGameModeBase::CalculateGame()
{
	if (player1Score == 10)
	{
		playerWin = 1;
	}
	else if (player2Score == 10)
	{
		playerWin = 2;
	}
}



