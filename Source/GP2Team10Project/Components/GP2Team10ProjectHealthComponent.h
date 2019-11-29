#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GP2Team10ProjectHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeathEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthEvent, int, Value);

UCLASS(Blueprintable)
class GP2TEAM10PROJECT_API UGP2Team10ProjectHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGP2Team10ProjectHealthComponent();

	// Get the current health of the character
	UFUNCTION(BlueprintPure, Category = "Health")
		bool GetInvincible() const;

	// Get the current health of the character
	UFUNCTION(BlueprintPure, Category = "Health")
		int GetCurrentHealth() const;

	// Get the current health of the character
	UFUNCTION(BlueprintPure, Category = "Health")
		int GetMaxHealth() const;

	// Get the current health of the character
	UFUNCTION(BlueprintCallable, Category = "Health")
		void ModifyMaxHealth(int Value);

	// Add value to CurrentHealth
	UFUNCTION(BlueprintCallable, Category = "Health")
		void ModifyHealth(int Value);

	// Add value to CurrentHealth
	UFUNCTION(BlueprintCallable, Category = "Health")
		void DecreaseHealth(int Value);

	// Add value to CurrentHealth
	UFUNCTION(BlueprintCallable, Category = "Health")
		void IncreaseHealth(int Value);

	// Fires when damage is recieved
	UPROPERTY(BlueprintAssignable, Category = "Health")
		FHealthEvent OnDamageTaken;

	// Fires when health reaches zero
	UPROPERTY(BlueprintAssignable, Category = "Health")
		FDeathEvent OnHealthZero;

protected:
	// The current value of heatlh
	UPROPERTY(VisibleAnywhere, Category = "Health")
		int CurrentHealth;

	// The max health of the component
	UPROPERTY(EditDefaultsOnly, Category = "Health")
		int MaxHealth = 3;

	// Invincible flag - Invincibles takes damage but doesnt lose health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool bInvincible = false;

	// Add value to CurrentHealth
	UFUNCTION(BlueprintCallable, Category = "Health")
		void ToggleInvincibilityOff();

private:
	FTimerHandle InvincibleTimer;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Event called when collision happens
	UFUNCTION()
		void OnCollisionEnter(AActor* Self, AActor* Other, FVector NormalImpulse, const FHitResult& Hit);
};
