// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP2Team10ProjectExplosion.generated.h"


UCLASS()
class GP2TEAM10PROJECT_API AGP2Team10ProjectExplosion : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP2Team10ProjectExplosion();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AddPushback(TArray<AActor*> OutActors);

	void DealDamage(AActor* HitActor);

	TArray<AActor*> CheckForActorsInExplosionRadius();

	void PlayExplosionSound();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion")
		float ExplosionRadius = 500;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion")
		float ExplosionStrength;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion")
		float ExplosionScale;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Explosion")
		float ExplosionDamage = 1;

	/** Audio event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODEvent* ExplosionSound;
};
