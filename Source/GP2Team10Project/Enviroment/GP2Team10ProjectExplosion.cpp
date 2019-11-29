#include "GP2Team10ProjectExplosion.h"
#include <Kismet/GameplayStatics.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "Components/GP2Team10ProjectHealthComponent.h"
#include "Player/GP2Team10ProjectPlayerCharacter.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

// Sets default values
AGP2Team10ProjectExplosion::AGP2Team10ProjectExplosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ExplosionScale = 10.f;
	ExplosionRadius = 500.f;
	ExplosionStrength = 2000.f;
}

// Called when the game starts or when spawned
void AGP2Team10ProjectExplosion::BeginPlay()
{
	Super::BeginPlay();

	// Todo: Found during review split program into different functions for increased readability/clarity compared to one massive block of code.
	PlayExplosionSound();

	TArray<AActor*> ActorsInExplosion = CheckForActorsInExplosionRadius();

	if (ActorsInExplosion.Num() > 0)
		AddPushback(ActorsInExplosion);
}

TArray<AActor*> AGP2Team10ProjectExplosion::CheckForActorsInExplosionRadius()
{
	TArray<FHitResult> OutHits;

	TArray<TEnumAsByte< EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery5);
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery6);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<AActor*> HitActors;

	FVector MyLocation = GetActorLocation();

	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(ExplosionRadius);

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), MyLocation, ExplosionRadius, ObjectTypes, nullptr, ActorsToIgnore, HitActors);

	return HitActors;
}

void AGP2Team10ProjectExplosion::AddPushback(TArray<AActor*> HitActors)
{
	FVector MyLocation = GetActorLocation();

	for (auto& HitActor : HitActors)
	{
		UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(HitActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

		if (MeshComp)
		{
			MeshComp->AddRadialImpulse(MyLocation, ExplosionRadius, ExplosionStrength, ERadialImpulseFalloff::RIF_Linear, true);
		}

		if (HitActor->GetClass()->IsChildOf<AGP2Team10ProjectPlayerCharacter>())
		{
			AGP2Team10ProjectPlayerCharacter* PlayerCharacter = Cast<AGP2Team10ProjectPlayerCharacter>(HitActor);

			if (PlayerCharacter)
			{
				PlayerCharacter->GetMovementComponent()->AddRadialImpulse(MyLocation, ExplosionRadius, ExplosionStrength, ERadialImpulseFalloff::RIF_Linear, true);

				if (!PlayerCharacter->GetGliderStatus())
				{
					DealDamage(HitActor);
				}
			}
		}
	}
}

void AGP2Team10ProjectExplosion::DealDamage(AActor* HitActor)
{
	UGP2Team10ProjectHealthComponent* HealthComp = Cast<UGP2Team10ProjectHealthComponent>(HitActor->FindComponentByClass<UGP2Team10ProjectHealthComponent>());

	if (HealthComp)
	{
		HealthComp->DecreaseHealth(ExplosionDamage);
	}
}

void AGP2Team10ProjectExplosion::PlayExplosionSound()
{
	if (ExplosionSound)
		UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), ExplosionSound, GetActorTransform(), true);
}
