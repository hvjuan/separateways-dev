// Juan Hernandez-Vargas
// Estudio2 Games.

#include "SplineTool.h"

#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Pawn.h" 

ASplineTool::ASplineTool()
{
	PrimaryActorTick.bCanEverTick = true;
	// Spline itself.
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	RootComponent = SplineComponent;
	// Box that triggers the spline to relocate its meshes across the spline.
	RelocationBox = CreateDefaultSubobject<UBoxComponent>(TEXT("RelocationBox"));
	RelocationBox->SetupAttachment(SplineComponent);
	// TODO: Set this to the mesh size.
	RelocationBox->SetBoxExtent(FVector(.5f, 40.f, 20.f));
	RelocationBox->SetMobility(EComponentMobility::Movable);
	RelocationBox->SetCollisionEnabled((ECollisionEnabled::QueryOnly));
	RelocationBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	RelocationBox->SetCollisionProfileName(TEXT("OverlapAll"));
	RelocationBox->OnComponentBeginOverlap.AddDynamic(this, &ASplineTool::OnOverlapBegin);
	RelocationBox->OnComponentEndOverlap.AddDynamic(this, &ASplineTool::OnOverlapEnd);
	RelocationBox->SetGenerateOverlapEvents(true);
	
	// We need to define a spline at the same time that will run underneath, creating a collision box
	// right below each mesh to allow it to end the game if the ball falls.
}

void ASplineTool::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(OverlapPawn))
	{
		bIsBallOverlapping = true;
		OnBallOverlapChanged(bIsBallOverlapping);
	}
}

void ASplineTool::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(OverlapPawn))
	{
		bIsBallOverlapping = false;
		OnBallOverlapChanged(bIsBallOverlapping);
	}
}

// Overlapping the boxes under the track to end the game.
// Takes a dynamic pawn
void ASplineTool::OnOverlapBeginEndGame(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(OverlapPawn))
	{
		OnBallOverlapGameEnds(true);
	}
}

void ASplineTool::InitSplineMeshPool()
{
	// Pre-allocate spline meshes in the pool.
	if (SplineMeshPool.Num() < MaxSplineMeshes)
	{
		for (int i = 0; i < MaxSplineMeshes; i++)
		{
			USplineMeshComponent* NewMesh = CreateSplineMesh();
			SplineMeshPool.Add(NewMesh);


			// Get the spline mesh size to define the box dimensions.
			FBox MeshBoundingBox = Mesh->GetBoundingBox();
			FVector MeshSize = MeshBoundingBox.GetSize();
			
			// TODO: Add to its own function.
			// Adding to the relocation box.
			UBoxComponent* LocalBox = NewObject<UBoxComponent>(this);
			LocalBox->SetupAttachment(SplineComponent);
			// TODO: Set to be bigger than the mesh itself so when the ball falls, it can get caught.
			LocalBox->SetBoxExtent(FVector(MeshSize.X / 2, MeshSize.Y * 2, 20.f));
			LocalBox->SetMobility(EComponentMobility::Movable);
			LocalBox->SetCollisionEnabled((ECollisionEnabled::QueryOnly));
			LocalBox->SetCollisionResponseToAllChannels(ECR_Overlap);
			LocalBox->SetCollisionProfileName(TEXT("OverlapAll"));
			LocalBox->SetVisibility(true);
			LocalBox->OnComponentBeginOverlap.AddDynamic(this, &ASplineTool::OnOverlapBeginEndGame);
			LocalBox->SetGenerateOverlapEvents(true);
			FallBoxesPool.Add(LocalBox);
		}
	}
}

void ASplineTool::DestroyPoolMeshes()
{
	for (auto LocalMesh: SplineMeshPool)
	{
		if (LocalMesh)
		{
			LocalMesh->DestroyComponent();
		}
	}
	SplineMeshPool.Empty();
	// Destroy Boxes.
	for (auto LocalBox: FallBoxesPool)
		if (LocalBox)
		{
			LocalBox->DestroyComponent();
		}
	FallBoxesPool.Empty();
}

void ASplineTool::ClearSplineMeshes() const
{
	for (int32 i = SplineComponent->GetNumChildrenComponents() -1; i >= 0; i--)
	{
		USceneComponent* ChildComponent = SplineComponent->GetChildComponent(i);
		if (ChildComponent && ChildComponent->IsA<USplineMeshComponent>())
		{
			ChildComponent->DestroyComponent();		
		}
	}
}

void ASplineTool::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("ASplineTool::BeginPlay"));
	if (bEnablePool)
	{
		InitSplineMeshPool(); 
	}
}

void ASplineTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SplineComponent->GetNumberOfSplinePoints() > 0)
	{
		if (bEnableAutomaticGrowth)
		{
			ElapsedTime += DeltaTime;
			float DeltaY = YAmplitude * FMath::Cos(ElapsedTime * YMovementSpeed);
			float DeltaZ = ZAmplitude * FMath::Sin(ElapsedTime * ZMovementSpeed);
			
			int LastPointIndex = SplineComponent->GetNumberOfSplinePoints() - 1;
			FVector CurrentPosition = SplineComponent->GetLocationAtSplinePoint(
				LastPointIndex, ESplineCoordinateSpace::Local);

			// Growing spline object.
			FVector NewPosition = CurrentPosition + FVector(ForwardSpeed * DeltaTime, DeltaY, DeltaZ);
			SplineComponent->SetLocationAtSplinePoint(LastPointIndex, NewPosition, ESplineCoordinateSpace::Local);
		}
		// If the growth is not set, just render it.
		BaseSpline();
	}
}

void ASplineTool::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	// Clearing the array.
	if (Mesh)
	{
		if (bEnablePool)
		{
			// Clear them so when we switch to performant, everything gets deleted.
			// Do on each run so it redraws it every frame.
			ClearSplineMeshes();
			
			// Make sure we don't crash when we enable the mesh pool.
			if (!SplineMeshPool.IsEmpty()) DestroyPoolMeshes();

			// Initialize so it renders.
			InitSplineMeshPool();
			
			if (SplineMeshPool.Num() == 0)
			{
				UE_LOG(LogTemp, Error, TEXT("SplineMeshPool failed to initialize properly"));
				return;
			}
		}
		else
		{
			ClearSplineMeshes();		
		}
        BaseSpline();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("onConstruction: Spline doesn't have a mesh!"));
	}
}

USplineMeshComponent* ASplineTool::CreateSplineMesh()
{
	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(
		this, USplineMeshComponent::StaticClass());
	
	// Setting up mesh.
	SplineMesh->SetForwardAxis(MeshForwardAxis);
	SplineMesh->SetStaticMesh(Mesh);
	SplineMesh->CreationMethod = EComponentCreationMethod::Instance;
	SplineMesh->SetMobility(EComponentMobility::Movable);
		
	// Setting collision.
	if (bEnableCollision)
	{
		SplineMesh->SetCollisionProfileName(TEXT("BlockAll"));
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SplineMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	}

	SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	// Visibility.
	SplineMesh->SetVisibility(true);
	SplineMesh->SetHiddenInGame(false); 
	SplineMesh->RegisterComponentWithWorld(GetWorld());
	
	return SplineMesh;
}

void ASplineTool::UpdateSplineMeshLocations(
	int CurrentSpline,
	USplineMeshComponent* SplineMesh,
	float MeshXLength,
	UBoxComponent* CollisionBox,
	UBoxComponent* FallBoxComponent)
{
	// Define the positions of the points and tangents.
	// Multiply the index by the XLength.
	float SegmentStart = CurrentSpline * MeshXLength;
	float SegmentEnd = SegmentStart + MeshXLength;

	// Starting spline render.
	const FVector StartPoint = SplineComponent->GetLocationAtDistanceAlongSpline(
		SegmentStart, ESplineCoordinateSpace::Local);
	const FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(
		SegmentStart, ESplineCoordinateSpace::Local);
	// Clamping vector size for tangents.
	const FVector ClampedStartTangent = StartTangent.GetClampedToMaxSize(MeshXLength);
	// Ending spline render.
	const FVector EndPoint = SplineComponent->GetLocationAtDistanceAlongSpline(
		SegmentEnd, ESplineCoordinateSpace::Local);
	const FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(
		SegmentEnd, ESplineCoordinateSpace::Local);
	// Clamping vector size for tangents.
	const FVector ClampedEndTangent = EndTangent.GetClampedToMaxSize(MeshXLength);

	FVector BoxLocation = SplineComponent->GetLocationAtDistanceAlongSpline(SegmentStart, ESplineCoordinateSpace::World);

	// Rendering...
	SplineMesh->SetStartAndEnd(
		StartPoint, ClampedStartTangent, EndPoint, ClampedEndTangent);

	// Setting the collision box.
	if (CollisionBox)
	{
		CollisionBox->SetWorldLocation(BoxLocation);
	}
	
	if (FallBoxComponent)
	{
		// Update location.
		if (Mesh)
		{
			BoxLocation += FVector(0.f, 0.f, -300.f);
			FallBoxComponent->SetWorldLocation(BoxLocation);
		}
	}
}

void ASplineTool::BaseSpline()
{
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("Spline Tool: Mesh is null"));
		return;
	}

	int TotalSplinePoints = SplineComponent->GetNumberOfSplinePoints();

	// Get mesh bounding boxes to properly create the spline and its edges.
	FBox BoundingBox = Mesh->GetBoundingBox();
	FVector MinAbs = BoundingBox.Min.GetAbs();
	FVector MaxAbs = BoundingBox.Max.GetAbs();
	float XLength = MinAbs.X + MaxAbs.X;

	// Get the spline length and divide it by the bounding box so it becomes the upper bound.
	float SplineLength = SplineComponent->GetSplineLength();
	int SplineTotal = FMath::CeilToInt(SplineLength / (XLength + TotalSplinePoints));

	// Setting the collision box. I will be using RightMost and LeftMost to determine the half of the rendered
	// meshes. This will be used to change these variables.
	// ie: 10 / 2 = 5. In this scenario, the collision box will be rendered in the 5th position and add
	// its value (5) to RightMost and LeftMost, and set the new half.
	int HalfManual = ((RightMost - LeftMost) / 2) + LeftMost;
	
	UBoxComponent* CollisionBox = nullptr;
	
	for (int Segment = 0; Segment < SplineTotal; Segment++)
	{
		// Last mesh. If enabled, the last mesh will not be rendered because for some meshes
		// the last position can make it look weird if the size is smaller than its X. 
		if (bKeepCompleteShape && Segment == SplineTotal - 1) continue;

		// Check if performance is set or not. This means that if it's set, the mesh pool will be enabled
		// and kept across the spline.
		if (bEnablePool)
		{
			// Growth.
			// Rendering just the meshes. Thing is that this doesn't work on game mode as the spline is not rebuilt.
			// What seems to be the solution is to physically keep a pool of meshes and change their location as the
			// actor moves forward.

			// Manual relocation of meshes.
			if (bEnableManualPlacing)
			{
				// We need to make sure that RightMost is always less than SplineTotal.
				if (Segment < LeftMost || Segment >= RightMost)
				{
					// If not within the manual range, skip it.
					continue;
				}
				// Relocate existing collision box.
				CollisionBox = Segment == HalfManual ? RelocationBox : nullptr;
			}
			const int MeshIndex = Segment < SplineMeshPool.Num() ? Segment : Segment % SplineMeshPool.Num();
			// UpdateSplineMeshLocations(Segment, SplineMeshPool[MeshIndex], XLength, CollisionBox);
			UpdateSplineMeshLocations(
				Segment,
				SplineMeshPool[MeshIndex],
				XLength,
				CollisionBox,
				FallBoxesPool[MeshIndex]);
		}
		// Render across the spline.
		else
		{
			UpdateSplineMeshLocations(Segment, CreateSplineMesh(), XLength, nullptr, nullptr);
		}
	}
}