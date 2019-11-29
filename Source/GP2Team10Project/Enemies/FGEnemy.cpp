#include "FGEnemy.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/GP2Team10ProjectPlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "FGEnemySpawner.h"
#include "Components/CapsuleComponent.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
// Todo: Found during review, removed includes not being used for optimization.

// Sets default values
AFGEnemy::AFGEnemy()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("RootCollision");
	StaticMesh->SetCollisionProfileName("BlockAll");
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetEnableGravity(false);
	RootComponent = StaticMesh;

	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleCollision");
	CapsuleCollision->SetupAttachment(StaticMesh);
	CapsuleCollision->OnComponentHit.AddDynamic(this, &AFGEnemy::OnHit);

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SkeletalMesh->SetCollisionProfileName("NoCollision");
	SkeletalMesh->SetupAttachment(RootComponent);

	AggroRange = CreateDefaultSubobject<USphereComponent>("AggroRange");
	AggroRange->SetupAttachment(RootComponent);
	AggroRange->SetSphereRadius(500.f);
	AggroRange->OnComponentBeginOverlap.AddDynamic(this, &AFGEnemy::HandleAggroEnter);
	AggroRange->OnComponentEndOverlap.AddDynamic(this, &AFGEnemy::HandleAggroExit);

	//Todo: Found using visual studio to look for unused variables, removed for cleanliness/optimization.
}

void AFGEnemy::SetSpawner(AFGEnemySpawner* SP)
{
	if (SP != nullptr)
	{
		Spawner = SP;
	}
}

//Todo: Found using visual studio method checking, removed since function does nothing 

// Called when the game starts or when spawned
void AFGEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFGEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target != nullptr)
	{
		FVector LookDirection = Target->GetActorLocation() - GetActorLocation();
		float DistanceToTarget = LookDirection.Size();


		// Todo: Found during review, removed old commented out code for clarity.

		if (DistanceToTarget > ChaseDistance)
		{
			LookDirection.Normalize();
			AddActorWorldOffset(LookDirection * MovementSpeed * DeltaTime);
		}
		else
		{
			LookDirection.Normalize();
			AddActorWorldOffset(LookDirection * MovementSpeed * DeltaTime);
		}

		FRotator LookRotator = FRotationMatrix::MakeFromX(LookDirection).Rotator();
		SetActorRotation(LookRotator);


		// Todo: Found during playtest/review, code is nonsense that does nothing, removed for optimization/cleanliness
	}

}

void AFGEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor->ActorHasTag(FName(TEXT("Rope"))))
	{
		//Todo: found during code review, removed code duplication for simplification 
		if (Spawner)
		{
			Spawner->EnemyIsDead = true;
			Spawner->SpawnEnemy();
		}
			

		Destroy();
	}
}

void AFGEnemy::HandleAggroEnter(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AGP2Team10ProjectPlayerCharacter>())
	{

		Target = Cast<AGP2Team10ProjectPlayerCharacter>(OtherActor);

		UFMODBlueprintStatics::PlayEventAttached(MineAggroSound, RootComponent, NAME_None, RootComponent->GetComponentLocation(), EAttachLocation::KeepWorldPosition, true, true, true);
	}

}

void AFGEnemy::HandleAggroExit(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor == Target)
	{
		Target = nullptr;
	}
}


