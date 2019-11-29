// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Curves/CurveFloat.h"
#include "FGEnemyProjectile.generated.h"


class USphereComponent;
class AGP2Team10ProjectPlayerCharacter;


UCLASS()
class GP2TEAM10PROJECT_API AFGEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFGEnemyProjectile();
	
	void BeginPlay() override;

	UFUNCTION()
		void HandleSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
		void OnProjectileHit(AActor* HitActor, FVector HitLocation);
	virtual void OnProjectileHitInternal(AActor* HitActor, FVector HitLocation);

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		float ProjectileSpeed = 2.f;
	UPROPERTY(EditDefaultsOnly)
	float maxRange = 1000.f;

	UPROPERTY(EditDefaultsOnly)
		UCurveFloat* VelocityCurve;

public:
	UPROPERTY(VisibleAnywhere)
		USphereComponent* SphereCollision;

	void Shoot(float Range);


	AGP2Team10ProjectPlayerCharacter* Target = nullptr;
};
