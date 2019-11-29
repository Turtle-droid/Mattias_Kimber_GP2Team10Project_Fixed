#include "GP2Team10ProjectHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Enemies/FGEnemyProjectile.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UGP2Team10ProjectHealthComponent::UGP2Team10ProjectHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Get the owner of the component
	AActor* Owner = GetOwner();

	if (Owner)
	{
		Owner->OnActorHit.AddDynamic(this, &UGP2Team10ProjectHealthComponent::OnCollisionEnter);
	}
}

// Called when the game starts
void UGP2Team10ProjectHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UGP2Team10ProjectHealthComponent::OnCollisionEnter(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Other)
	{
		if (Other->GetClass()->IsChildOf<AFGEnemyProjectile>())
		{
			DecreaseHealth(1);

			Other->Destroy();
		}
	}
}

bool UGP2Team10ProjectHealthComponent::GetInvincible() const
{
	return bInvincible;
}

int UGP2Team10ProjectHealthComponent::GetCurrentHealth() const
{
	return CurrentHealth;
}

int UGP2Team10ProjectHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

void UGP2Team10ProjectHealthComponent::ModifyMaxHealth(int Value)
{
	MaxHealth = Value;
}

void UGP2Team10ProjectHealthComponent::ModifyHealth(int Value)
{
	CurrentHealth = Value;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
}

void UGP2Team10ProjectHealthComponent::DecreaseHealth(int Value)
{
	if (!bInvincible)
	{
		bInvincible = true;
		CurrentHealth -= Value;
		if (CurrentHealth <= 0)
		{
			OnHealthZero.Broadcast();
		}
		GetWorld()->GetTimerManager().SetTimer(InvincibleTimer, this, &UGP2Team10ProjectHealthComponent::ToggleInvincibilityOff, 1.0f, false);
		CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
		OnDamageTaken.Broadcast(Value);
	}
}

void UGP2Team10ProjectHealthComponent::IncreaseHealth(int Value)
{
	if (CurrentHealth < MaxHealth)
	{
		CurrentHealth += Value;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
	}
}

void UGP2Team10ProjectHealthComponent::ToggleInvincibilityOff()
{
	bInvincible = false;
}

