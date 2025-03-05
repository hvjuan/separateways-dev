
#include "GameSplitGameMode.h"

#include "Ball.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AGameSplitGameMode::AGameSplitGameMode()
{
}

void AGameSplitGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AGameSplitGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	AddLocalPlayers();
}

void AGameSplitGameMode::AddLocalPlayers()
{
	// Get all player controllers in the world
	TArray<APlayerController*> PlayerControllers;
	
	// Create 2 players.
	UE_LOG(LogTemp, Error, TEXT("About to start creating players!"));

	/*
	// Check if there's a valid world.
	if (UWorld* World = GetWorld())
	{
		// Create a new local player with controller for the second player.
		if (APlayerController* NewPlayerController = UGameplayStatics::CreatePlayer(World, -1, true))
		{
			UE_LOG(LogTemp, Warning, TEXT("Player controller created for player 2, Name: %s."), *NewPlayerController->GetName());
		}
	}
	*/	
}
