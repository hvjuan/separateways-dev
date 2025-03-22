// Juan Hernandez-Vargas
// Estudio2 Games.

#include "GameSplitGameModeCPP.h"

#include "Ball.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AGameSplitGameModeCPP::BeginPlay()
{
	Super::BeginPlay();

	// Get all actors of class Player Start. This will later be used to get its location->transform when
	// spawning the balls.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
	if (PlayerStarts.Num() < 2)
	{
		UE_LOG(LogTemp, Error, TEXT("Not enough Player Starts found in the scene"));
		return;
	}
	// TArray<FTransform*> PlayerStartTransforms;
	// We need to spawn the ball as the left player first. This requires that we have set two player starts
	// on the level
	FTransform LeftTransform = PlayerStarts[0]->GetActorTransform();
	FTransform RightTransform = PlayerStarts[1]->GetActorTransform();
	UWorld* World = GetWorld();
	UGameInstance* GameInstance = World->GetGameInstance();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;

	// Debug log for player start transforms
	UE_LOG(LogTemp, Warning, TEXT("Left Player Start Transform: %s"), *LeftTransform.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Right Player Start Transform: %s"), *RightTransform.ToString());
	
	// Spawn the first ball, left ball.
	ABall* LeftBall = World->SpawnActor<ABall>(BallBlueprintClass, LeftTransform, SpawnParameters);
	if (APlayerController* LeftBallPlayerController = World->GetFirstPlayerController())
	{
		LeftBallPlayerController->Possess(LeftBall);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Left Controller Not Found"));
	}
	
	// Create the second controller and spawn the right ball.
	FString ErrorMessage;
	if (ULocalPlayer* RightPlayer = GameInstance->CreateLocalPlayer(RightBallControllerId, ErrorMessage, true))
	{
		ABall* RightBall = World->SpawnActor<ABall>(BallBlueprintClass, RightTransform, SpawnParameters);
		if (AController* RightPlayerController = RightPlayer->PlayerController)
		{
			RightPlayerController->Possess(RightBall);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Right Controller Not Found"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error: %s"), *ErrorMessage);
	}
	
}
