// Todo: Found looking for refrences in unreal, consider removal since it's not in use for optimization.

#include "FGRangedEnemy.h"
#include "Components/BoxComponent.h"
#include "Components//SphereComponent.h"
#include "FGEnemyProjectile.h"
#include "GameFramework/DamageType.h"
#include "Player/GP2Team10ProjectPlayerCharacter.h"
#include "Kismet/GameplayStaticsTypes.h"

// Sets default values
AFGRangedEnemy::AFGRangedEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp = CreateDefaultSubobject<UBoxComponent>("RootCollison");
	RootComponent = BoxComp;
	BoxComp->SetCollisionProfileName("BlockAll");

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	//RootComponent = StaticMesh;
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName("BlockAll");
	/*StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetEnableGravity(false);*/

	AggroRange = CreateDefaultSubobject<USphereComponent>("AggroRange");
	AggroRange->SetupAttachment(BoxComp);
	AggroRange->SetSphereRadius(1000.f);
	AggroRange->OnComponentBeginOverlap.AddDynamic(this, &AFGRangedEnemy::HandleAggroEnter);
	AggroRange->OnComponentEndOverlap.AddDynamic(this, &AFGRangedEnemy::HandleAggroExit);

	AttackSpeed = 10;
	AttackDamage = 100;


}

// Called when the game starts or when spawned
void AFGRangedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFGRangedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	

	if (Target != nullptr)
	{
		FVector LookDirection = Target->GetActorLocation() - GetActorLocation();
		float DistanceToTarget = LookDirection.Size();

		if (DistanceToTarget > ChaseDistance)
		{
			LookDirection.Normalize();
			AddActorWorldOffset(-LookDirection * DeltaTime);
		}
		else
		{
			LookDirection.Normalize();
			AddActorWorldOffset(LookDirection * DeltaTime);
		}

		FRotator LookRotator = FRotationMatrix::MakeFromX(LookDirection).Rotator();
		// Here we make the Enemy move 100 frame per second towards us //
		//SetActorRotation(LookRotator);


		AttackSpeed += DeltaTime;
		if (AttackSpeed > 4.f)
		{
			//FRotator LookRotator = FQuat::FindBetweenVectors();
			FVector SpawnLocation = GetActorLocation() + LookRotator.Vector() * 150.f;
			AFGEnemyProjectile* projectile = GetWorld()->SpawnActor<AFGEnemyProjectile>(ProjectileClass, StaticMesh->GetSocketTransform("BulletHole").GetLocation(), LookRotator);
			projectile->Target = Target;
			//projectile->maxRange = AggroRange->GetScaledSphereRadius();
			projectile->Shoot(AggroRange->GetScaledSphereRadius());
			AttackSpeed = 2.f;
		}
	}
	

}

void AFGRangedEnemy::HandleAggroEnter(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AGP2Team10ProjectPlayerCharacter>())
	{
		Target = Cast<AGP2Team10ProjectPlayerCharacter>(OtherActor);

		//Todo: Found during gameplay, removed debuglog message due to it being clutter 

	}
}

void AFGRangedEnemy::HandleAggroExit(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor == Target)
	{
		Target = nullptr;
	}

	//Todo: Found during gameplay, removed debuglog message due to it being clutter 
}

