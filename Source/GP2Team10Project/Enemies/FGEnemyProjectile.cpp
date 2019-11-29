// Todo: Found looking for refrences in unreal, consider removal since it's not in use for optimization.

#include "FGEnemyProjectile.h"
#include "Components/SphereComponent.h"
#include "Player/GP2Team10ProjectPlayerCharacter.h"


// Sets default values
AFGEnemyProjectile::AFGEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>("RootCollision");
	RootComponent = SphereCollision;

	SphereCollision->SetCollisionProfileName("BlockAll");
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	SphereCollision->SetSimulatePhysics(true);
	SphereCollision->bHiddenInGame = false;
	SphereCollision->SetNotifyRigidBodyCollision(true);
	SphereCollision->OnComponentHit.AddDynamic(this, &AFGEnemyProjectile::HandleSphereHit);

}


void AFGEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();
	//SphereCollision->AddImpulse(GetActorForwardVector() * 1200.f, NAME_None, true);
	//SphereCollision->AddImpulse(GetActorUpVector() * 500.f, NAME_None, true);
}


void AFGEnemyProjectile::HandleSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnProjectileHitInternal(OtherActor, Hit.Location);
	OnProjectileHit(OtherActor, Hit.Location);
}

void AFGEnemyProjectile::OnProjectileHitInternal(AActor* HitActor, FVector HitLocation)
{

}

void AFGEnemyProjectile::Shoot(float Range)
{
	FVector LookDirection = Target->GetActorLocation() - GetActorLocation();
	FRotator LookRotator = FRotationMatrix::MakeFromX(LookDirection).Rotator();
	float distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());

	float distancePercentage = distance / Range;
	UE_LOG(LogTemp, Error, TEXT("Distance: %f    Percent: %f"), distancePercentage, (VelocityCurve->GetFloatValue(distancePercentage) * maxRange));
	SphereCollision->AddImpulse(LookRotator.Vector() * VelocityCurve->GetFloatValue(distancePercentage) * maxRange, NAME_None, true);
}

void AFGEnemyProjectile::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Error, TEXT("%f"), this->GetVelocity().Size());
	//if (this->GetVelocity().Size() < 100.0f)
	//{
	//	// Todo Homing missile.
	//	FVector LookDirection = Target->GetActorLocation() - GetActorLocation();
	//	FRotator LookRotator = FRotationMatrix::MakeFromX(LookDirection).Rotator();
	//	float distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
	//	
	//	float distancePercentage = distance / maxRange;
	//	SphereCollision->AddImpulse(LookRotator.Vector() * VelocityCurve->GetFloatValue(distancePercentage) * maxRange, NAME_None, true);
	//}
}

// Called when the game starts or when spawned
