// Juan Hernandez-Vargas
// Estudio2 Games.

#include "ControllersCPP.h"

#include "Ball.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

void AControllersCPP::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> FoundBalls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABall::StaticClass(), FoundBalls);
	UE_LOG(LogTemp, Warning, TEXT("Before the for in controller"));
	if (FoundBalls.Num() == 2)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			AActor* RightBallActor = FoundBalls[1];
			ABall* RightBall = Cast<ABall>(RightBallActor);
			EnhancedInputComponent->BindAction(RightBallSidesMoveAction, ETriggerEvent::Triggered, RightBall, &ABall::BallSidesMove);
		}
	}
}
