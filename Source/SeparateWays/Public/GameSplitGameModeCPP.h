// Juan Hernandez-Vargas
// Estudio2 Games.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameSplitGameModeCPP.generated.h"

/**
 * 
 */
UCLASS()
class SEPARATEWAYS_API AGameSplitGameModeCPP : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Ball Players")
	TSubclassOf<class ABall> BallBlueprintClass;
	
protected:
	TArray<AActor*> PlayerStarts;
	private: const int LeftBallControllerId = 0;
	const int RightBallControllerId = 1;
};
