// Fill out your copyright notice in the Description page of Project Settings.

#include "FGEnemySpawner.h"
#include "Components/BoxComponent.h"
#include "FGEnemy.h"

// Sets default values
AFGEnemySpawner::AFGEnemySpawner()
{
	// Todo: disabled tick during rewrite for optimization.
	PrimaryActorTick.bCanEverTick = false;
	BoxSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
}

void AFGEnemySpawner::SpawnEnemy()
{
	if (!EnemyIsDead) {
		return;
	}
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = this;


	AFGEnemy* Enemy = GetWorld()->SpawnActor<AFGEnemy>(FGEnemy1, GetActorLocation(), GetActorRotation(), Params);

	if (Enemy != nullptr)
	{
		BP_FGEnemySpawned(Enemy);
		Enemy->SetSpawner(this);
		EnemyIsDead = false;
	}
}

//Todo: Removed usage of tick function since its wasteful when objects should be able to communicate when a new enemy should spawn rather then constanst checking. 

// Called when the game starts or when spawned
void AFGEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemy();
}