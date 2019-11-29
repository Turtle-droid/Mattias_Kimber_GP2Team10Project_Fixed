// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/GP2Team10ProjectPlayerCharacter.h"
#include "FGEnemySpawner.generated.h"

class UBoxComponent;
class AFGEnemy;

UCLASS()
class GP2TEAM10PROJECT_API AFGEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFGEnemySpawner();

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* BoxSpawn;

	bool EnemyIsDead = true;

	//Todo: Found during game review, not actually being used and implementation is a resource hog, quick rewrite for optimization or simply DELETE THIS.
	void SpawnEnemy();

	//Todo: Found during review unused function removed for readability / compile time.

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = Spawning, meta = (DisplayName = "On Enemy Spawn"))
		void BP_FGEnemySpawned(AFGEnemy* FGEnemy);

	UPROPERTY(EditAnywhere, Category = Actor)
		TSubclassOf<AFGEnemy> FGEnemy1;

	//Todo: Removed during rework to get away from tick function for optimization.
};
