#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Player/GP2Team10ProjectPlayerCharacter.h"
#include "ManualPickup.generated.h"

class AGP2Team10ProjectPlayerCharacter;
class USphereComponent;

UCLASS()
class GP2TEAM10PROJECT_API AManualPickup : public AInteractable
{
	GENERATED_BODY()

public:
	AManualPickup();
	void Interact_Implementation(APlayerController* Controller) override;

private:
	UFUNCTION()
		void HandlePickupEnterRange(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void HandlePickupEndRange(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

	AGP2Team10ProjectPlayerCharacter* Target = nullptr;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName ItemID;


};