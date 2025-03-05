
#include "BallGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void ABallGameMode::BeginPlay()
{
	Super::BeginPlay();

	CreateLocalPlayer();
}

void ABallGameMode::CreateLocalPlayer()
{
	// Check if there's a valid world.
	UWorld* World = GetWorld();
	if (World)
	{
		// Create a new local player with controller for the second player.
		APlayerController* NewPlayerController = UGameplayStatics::CreatePlayer(World, -1, true);
		if (NewPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("Player controller created for player 2."));
		}
	}

	// Check all pawns first
    TArray<APawn*> AllPawns;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), reinterpret_cast<TArray<AActor*>&>(AllPawns));
    
    UE_LOG(LogTemp, Warning, TEXT("Found %d pawns:"), AllPawns.Num());
    for (APawn* Pawn : AllPawns)
    {
        UE_LOG(LogTemp, Warning, TEXT("Pawn: %s, Possessed by: %s"), 
            *Pawn->GetName(), 
            Pawn->GetController() ? *Pawn->GetController()->GetName() : TEXT("None"));
    }
	
	// Learning how the player controllers are seen,
	TArray<APlayerController*> PlayerControllers;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		if (const APlayerController* PlayerController = Iterator->Get())
		{
			UE_LOG(LogTemp, Warning, TEXT("Player controller: %s"), *PlayerController->GetName());
			if (APlayerController* PC = Iterator->Get())
			{
				PlayerControllers.Add(PC);
				
				if (ULocalPlayer* LP = PC->GetLocalPlayer())
				{
					UE_LOG(LogTemp, Warning, TEXT("Local player: %s"), *LP->GetName());
					int32 ControllerId = LP->GetControllerId();
					// Get enhanced input subsystem.
					if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
					{
						if (PlayerInputContexts.IsValidIndex(ControllerId))
						{
							UE_LOG(LogTemp, Warning, TEXT("Adding mapping context for player %d..."), ControllerId);
							Subsystem->AddMappingContext(PlayerInputContexts[ControllerId], 0);
						}
					}
				}
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("There are %d Player controllers..."), PlayerControllers.Num());
}
