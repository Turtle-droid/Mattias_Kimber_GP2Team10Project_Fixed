#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemies/FGEnemy.h"
#include "GP2Team10ProjectSteamVent.generated.h"

UCLASS()
class GP2TEAM10PROJECT_API AGP2Team10ProjectSteamVent : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP2Team10ProjectSteamVent();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamVent")
		float AirPower = 750.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamVent")
		bool bDoReset = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamVent")
		bool bSteamVentActive = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		class USceneComponent* SteamPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerBox")
		class UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamVent")
		class AGP2Team10ProjectPressurePad* MyPressurePad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamVent")
		AActor* PlayerActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODEvent* SteamVentSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODAudioComponent* FMODSoundComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamVent")
		float LiftBoostInterval = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SteamVent")
		float LiftBoostRemainderTime = 0.5f;

	UFUNCTION(BlueprintImplementableEvent, Category = "SteamVent")
		void OnSteamVentActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "SteamVent")
		void OnSteamVentDeactivated();

	UFUNCTION(BlueprintCallable, Category = "SteamVent")
		void ToggleSteamVentState(bool bPressurePadDown);

private:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle SteamVentLiftTimerHandle;

	UFUNCTION()
		void OnLiftActor();

	UFUNCTION()
		void RemoveUpliftForce();

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
