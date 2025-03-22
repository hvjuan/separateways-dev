// Juan Hernandez-Vargas
// Estudio2 Games.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "ControllersCPP.generated.h"

/**
 * 
 */
UCLASS()
class SEPARATEWAYS_API AControllersCPP : public APlayerController
{
		GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category=Input)
    UInputAction* RightBallSidesMoveAction;
	
};
