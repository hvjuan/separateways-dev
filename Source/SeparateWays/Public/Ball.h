
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Ball.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;
class UStaticMeshComponent;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SEPARATEWAYS_API ABall : public APawn
{
	GENERATED_BODY()

public:
	ABall();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category=Mesh)
	UStaticMeshComponent* BallMesh;

protected:
	virtual void BeginPlay() override;
	void PossessedBy(AController* NewController);
	
	FVector GetMeshMiddlePoint() const;

	// Detecting controller type.
	int32 GetControllerType();

	USceneComponent* Root;
	USceneComponent* CameraRoot;

	// Capsule.
	
	UPROPERTY(EditAnywhere, Category=Capsule)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, Category=Capsule)
	float CapsuleHalfHeight = 50.f;

	UPROPERTY(EditAnywhere, Category=Capsule)
	float CapsuleRadius = 50.f;

	// Camera.
	
	UPROPERTY(EditAnywhere, Category=Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category=Camera)
	float CameraBoomLength = 300.f;
	
	UPROPERTY(EditAnywhere, Category=Camera)
	UCameraComponent* ViewCamera;

	// Speed and Movement

	// Max Ball Speed.
	UPROPERTY(EditAnywhere, Category=Speed, meta=(ToolTip="Max Speed in m/s"))
	float MaxSpeed = 1000.f;
	// Acceleration speed.
	UPROPERTY(EditAnywhere, Category=Speed, meta=(ToolTip="Acceleration Speed in m/s"))
	float Acceleration = 100.f;
	
	UPROPERTY(EditAnywhere, Category=Speed, meta=(ToolTip="Acceleration Speed in m/s"))
	float TurnAcceleration = 100.f;

	// Controller. Left - Main
	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* BallMoveContext;
	
	UPROPERTY(EditAnywhere, Category=Input)
    UInputAction* BallSidesMoveAction;
	
	UPROPERTY(EditAnywhere, Category=Input)
    UInputAction* RightBallSidesMoveAction;
	
	UPROPERTY(BlueprintReadOnly, Category = Speed)
	FVector Speed;

	UFUNCTION(BlueprintCallable, Category="Input")
	void BallSidesMove(const FInputActionValue& Value);

private:
	FVector InitialForwardVector;
};

