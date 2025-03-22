// Juan Hernandez-Vargas
// Estudio2 Games.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SplineTool.generated.h"

class UBoxComponent;
class UInstancedStaticMeshComponent;
class UStaticMeshComponent;
class USplineComponent;
class USplineMesh;
class UPrimitiveComponent;
class UStaticMesh;
class APawn;

UCLASS()
class SKAROUTILS_API ASplineTool : public AActor
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// Updates the meshes location from the pool.	
	void UpdateSplineMeshLocations(
		int CurrentSpline,
		USplineMeshComponent* SplineMesh,
		float MeshXLength,
		UBoxComponent* CollisionBox = nullptr,
		UBoxComponent* FallBoxComponent = nullptr);
	// Creates a spline mesh component.	
	USplineMeshComponent* CreateSplineMesh();
	// Initializes the mesh pool array.
	void InitSplineMeshPool();
	// Destroys the meshes inside the pool.
	void DestroyPoolMeshes();
	// Clears the spline.
	void ClearSplineMeshes() const;
	
private:
	float ElapsedTime = 0;
	float ElapsedSplinePointFrequency = 0;
	bool bChangeDirection = false;
	float CurveSpawnSpeed = 5.f;

public:
	ASplineTool();
	
	UFUNCTION(BlueprintCallable, Category = "Spline Engine")
	void BaseSpline();
	
	UPROPERTY(VisibleAnywhere, Category = "Spline Engine")
	USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, Category = "Spline Engine")
	UStaticMesh* Mesh;
	
	UPROPERTY(EditAnywhere, Category = "Spline Engine")
	UStaticMesh* SpawnTestMesh;

	// Enables collision detection across all the instantiated meshes on the spline.
	UPROPERTY(EditAnywhere, Category = "Spline Engine")
	bool bEnableCollision = false;

	// Render the partial mesh or wait till the segment is complete
	// to Spawn.
	UPROPERTY(EditAnywhere, Category = "Spline Engine")
	bool bKeepCompleteShape = false;

	// Sets the mesh axis.
	UPROPERTY(EditAnywhere, Category = "Spline Engine")
	TEnumAsByte<ESplineMeshAxis::Type> MeshForwardAxis;

	//
	// Spline Growth
	//
	// Grow on X speed.
	// Enable the spline to grow based on all the other parameters.
	UPROPERTY(EditAnywhere, Category = "Spline Engine|Spline Growth")
	bool bEnableAutomaticGrowth = true;
	
	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshPool;
	
	// Pool of Box Components that will be spawned/relocated under the meshes
	// and when overlapped by the ball actor, will end the game.
	UPROPERTY()
	TArray<UBoxComponent*> FallBoxesPool;
	
	UPROPERTY(
		EditAnywhere,
		Category = "Spline Engine|Spline Growth",
		meta = (EditCondition = "bEnableAutomaticGrowth", EditConditionHides))
	float ForwardSpeed = 0;

	// Cos() Movement speed.
	UPROPERTY(
		EditAnywhere,
		Category = "Spline Engine|Spline Growth",
		meta = (EditCondition = "bEnableAutomaticGrowth", EditConditionHides))
	float YMovementSpeed = 100.f;

	// Sin() Movement speed.
	UPROPERTY(
		EditAnywhere,
		Category = "Spline Engine|Spline Growth",
		meta = (EditCondition = "bEnableAutomaticGrowth", EditConditionHides))
	float ZMovementSpeed = 100.f;
	
	// Y Curve Amplitude.
	UPROPERTY(
		EditAnywhere,
		Category = "Spline Engine|Spline Growth",
		meta = (EditCondition = "bEnableAutomaticGrowth", EditConditionHides))
	float YAmplitude = 0;
	
	// Z Curve Amplitude.
	UPROPERTY(
		EditAnywhere,
		Category = "Spline Engine|Spline Growth",
		meta = (EditCondition = "bEnableAutomaticGrowth", EditConditionHides))
	float ZAmplitude = 0;

	//
	// Spline Pool.
	//
	// Choose performant mode, where the mesh pool will be initialized
	// or keep the meshes all across the spline.
	UPROPERTY(EditAnywhere, Category = "Spline Engine|Spline Pool")
	bool bEnablePool = true;

	// Number of meshes in the pool.
	UPROPERTY(
		EditAnywhere,
		Category = "Spline Engine|Spline Pool",
		meta = (EditCondition = "bEnablePool", EditConditionHides))
	int MaxSplineMeshes = 10;

	// Spline Pool - Inner Pool.
	// Enable inner loop: Manual relocation of meshes. Need to have the performant mode set.
	UPROPERTY(
		EditAnywhere,
		Category = "Spline Engine|Spline Pool|Manual Placing",
		meta = (EditCondition = "bEnablePool", EditConditionHides))
	bool bEnableManualPlacing = false;

	// Base inner pool: defines in the spline pool where the meshes will be relocated. Base value.
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Spline Engine|Spline Pool|Manual Placing",
		meta = (EditCondition = "bEnablePool && bEnableManualPlacing", EditConditionHides))
	int LeftMost = 0;
	
	// Base inner pool: defines in the spline pool where the meshes will be relocated. Top value.
	UPROPERTY(
		EditAnywhere,
		BlueprintReadWrite,
		Category = "Spline Engine|Spline Pool|Manual Placing",
		meta = (EditCondition = "bEnablePool && bEnableManualPlacing", EditConditionHides))
	int RightMost = 10;

	//
	// Relocation box.
	//	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Spline Engine|Spline Pool|Manual Placing")
	UBoxComponent* RelocationBox;

	// Pawn that will trigger the relocation of spline mesh instances.
	UPROPERTY(EditAnywhere, Category = "Spline Engine|Spline Pool|Manual Placing")
	TSubclassOf<APawn> OverlapPawn;

	UFUNCTION(Category = "Spline Engine|Spline Pool|Manual Placing")
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(Category = "Spline Engine|Spline Pool|Manual Placing")
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
	UFUNCTION(Category = "Spline Engine|Spline Pool|Manual Placing")
	void OnOverlapBeginEndGame(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(BlueprintReadOnly, Category = "Spline Engine|Spline Pool|Manual Placing")
	bool bIsBallOverlapping = false;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Spline Engine|Events")
	void OnBallOverlapChanged(bool isBallOverlapping);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Spline Engine|Events")
	void OnBallOverlapGameEnds(bool isBallOverlapping);
};
