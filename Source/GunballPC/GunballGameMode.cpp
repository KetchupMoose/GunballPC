// Fill out your copyright notice in the Description page of Project Settings.

#include "GunballPC.h"
#include "GunballGameMode.h"


// Called when the game starts or when spawned
void AGunballGameMode::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGunballGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunballGameMode::PostSeamlessTravel()
{
	Super::PostSeamlessTravel();
	ServerTravelled();
}

//implement CalculateScore
void AGunballGameMode::CalculateScore(int PlayerGoal)
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
void AGunballGameMode::CalculateGame()
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


void AGunballGameMode::ServerTravelled_Implementation()
{
	//blah
}

