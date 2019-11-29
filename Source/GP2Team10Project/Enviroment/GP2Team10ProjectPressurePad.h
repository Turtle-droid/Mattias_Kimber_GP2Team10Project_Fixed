#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "GP2Team10ProjectPressurePad.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPressurePadState, bool, bPressurePadState);

UCLASS()
class GP2TEAM10PROJECT_API AGP2Team10ProjectPressurePad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP2Team10ProjectPressurePad();

	FPressurePadState PressurePadState;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pressure Pad")
		float PressurePadLimit = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "TriggerBox")
		class UBoxComponent* TriggerBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
		class UStaticMeshComponent* PressurePadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODEvent* PressurePadActivationSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODEvent* PressurePadDeactivationSound;

	class UFMODAudioComponent* FMODSoundComponent;

	UFUNCTION(BlueprintImplementableEvent, Category = "Pressure Pad")
		void OnPressurePadActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Pressure Pad")
		void OnPressurePadDeactivated();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	bool bPressurePadActivated = false;

	virtual void BeginPlay() override;

	void RunActivationProcedure();

	bool CheckOverlappingComponents();

	bool CheckForSimulatingMesh(UPrimitiveComponent* Component);

	bool CheckForPlayer(UPrimitiveComponent* Component);
};
