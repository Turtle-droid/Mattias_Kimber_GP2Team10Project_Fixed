// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FGEnemy.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class AGP2Team10ProjectPlayerCharacter;
class USkeletalMeshComponent;
class AFGEnemySpawner;
class UCapsuleComponent;

UCLASS()
class GP2TEAM10PROJECT_API AFGEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFGEnemy();

	AFGEnemySpawner* Spawner = nullptr;
	void SetSpawner(AFGEnemySpawner* SP);

	// Todo: found using visual studio method reference, unused variable function removed for optimization/readability.

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleCollision;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* AggroRange;

	UPROPERTY(EditAnywhere)
	bool bIsDead;

	UPROPERTY(EditAnywhere, Category = "Chasing")
	float MovementSpeed = 150.f;

	UPROPERTY(EditAnywhere, Category = "Chasing")
	float ChaseDistance = 500.f;

	UPROPERTY(EditAnywhere, Category = "Explosion")
	float ExplosiveRange = 150.f;

	UPROPERTY(EditDefaultsOnly, Category = "ExplosionRadius")
	float ExplosionRadius = 300.f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns Mesh subobject **/
	FORCEINLINE  USkeletalMeshComponent* GetSkeletalMesh() const { return SkeletalMesh; }

	/** Returns Mesh subobject **/
	FORCEINLINE  UStaticMeshComponent* GetStaticMesh() const { return StaticMesh; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Todo: Found using visual studio to look for unused variables, removed for cleanliness/optimization.

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODEvent* MineAggroSound;
private:

	UFUNCTION()
	void HandleAggroEnter(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void HandleAggroExit(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	AGP2Team10ProjectPlayerCharacter* Target = nullptr;

	int ExplosionTime = 5.f;
};
