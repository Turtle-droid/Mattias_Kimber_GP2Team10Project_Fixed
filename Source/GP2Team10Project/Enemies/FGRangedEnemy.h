// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FGEnemyProjectile.h"
#include "Player/GP2Team10ProjectPlayerCharacter.h"
#include "FGRangedEnemy.generated.h"


class USphereComponent;
class UBoxComponent;
class AGP2Team10ProjectPlayerCharacter;
class AFGEnemyProjectile;



UCLASS()
class GP2TEAM10PROJECT_API AFGRangedEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFGRangedEnemy();

	UPROPERTY(Category = Mesh, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* AggroRange;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AFGEnemyProjectile> ProjectileClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		float AttackSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
		float AttackDamage;


	UPROPERTY(EditAnywhere, Category = "Chasing")
		float ChaseDistance = 200.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UFUNCTION()
		void HandleAggroEnter(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void HandleAggroExit(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	AGP2Team10ProjectPlayerCharacter* Target = nullptr;


};
