
#include "Ball.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
// #include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
// #include "Kismet/GameplayStatics.h"

// Sets default values
ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Sphere collision and set the capsule as root.
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(CapsuleHalfHeight);
	Capsule->SetCapsuleRadius(CapsuleRadius);
	Capsule->SetSimulatePhysics(true);
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Capsule->SetGenerateOverlapEvents(true);
	Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Capsule->SetupAttachment(GetRootComponent());

	// Mesh.
	BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
	BallMesh->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	BallMesh->SetupAttachment(Capsule);

	// Camera.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->TargetArmLength = CameraBoomLength;
	CameraBoom->SetWorldRotation(FRotator(-30.f, 0.f, 0.f));
	// Attach the spring arm to the mid-point of the mesh.
	CameraBoom->SetRelativeLocation(GetMeshMiddlePoint());
	// Making the camera follow the ball but not orbit around it.
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->SetupAttachment(Capsule);

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	// Getting the initial forward vector.
    InitialForwardVector = GetActorForwardVector();
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
	
	// Getting the right ball.	
	// RightBall = GetRightBall();

	// Left.
	/*
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Left Ball %s has PlayerController: %s"), *GetName(), *PlayerController->GetName());
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			AutoPossessPlayer = EAutoReceiveInput::Player0;
			Subsystem->AddMappingContext(BallMoveContext, 0);
		}
	}
	*/
}

void ABall::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	UE_LOG(LogTemp, Warning, TEXT("=== Ball %s being possessed by: %s ==="), 
		*GetName(), 
		NewController ? *NewController->GetName() : TEXT("None"));
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector ForwardVector = InitialForwardVector;
	ForwardVector.Normalize();

	FVector BallCurrentSpeed = Capsule->GetComponentVelocity();
	// Update the external object to be read in blueprints.
	Speed += BallCurrentSpeed;
	if (BallCurrentSpeed.X < MaxSpeed)
	{
		// Get the mass of the BallMesh compoent.
		float BallMass = BallMesh->GetMass();
		// Calculate the force in Newtons.
		FVector Force = ForwardVector * BallMass * Acceleration;
		// Apply the force to the capsule.
		Capsule->AddForce(Force);
	}
}

FVector ABall::GetMeshMiddlePoint() const
{
	// Get the bounds of the mesh and calculate the middle point.
	FBoxSphereBounds Bounds = Capsule->CalcBounds(GetTransform());
	return Bounds.Origin;
}

int32 ABall::GetControllerType()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball %s has no PlayerController!"), *GetName());
		return -1; // Use -1 as an error code
	}

	ULocalPlayer* LP = PlayerController->GetLocalPlayer();
	if (!LP)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball %s has no LocalPlayer!"), *GetName());
		return -1; // Use -1 as an error code
	}

	int32 Id = LP->GetControllerId();
	UE_LOG(LogTemp, Warning, TEXT("Ball %s GetControllerType returning ID: %d"), *GetName(), Id);
	return Id;
}

void ABall::BallSidesMove(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	UE_LOG(LogTemp, Warning, TEXT("BallSidesMove Bool: %d"), Value.Get<bool>());
	UE_LOG(LogTemp, Warning, TEXT("LeftBallSidesMove: %f"), DirectionValue);
	if (DirectionValue != 0.f)
    {
		const FVector RightVector = GetActorRightVector();
		const FVector DirectionTotal = RightVector * DirectionValue * TurnAcceleration;
		Capsule->AddForce(DirectionTotal);
    }
}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	/*
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("Setting up input for Left Ball: %s"), *GetName());
		EnhancedInputComponent->BindAction(BallSidesMoveAction, ETriggerEvent::Triggered, this, &ABall::BallSidesMove);
	}
	*/
}