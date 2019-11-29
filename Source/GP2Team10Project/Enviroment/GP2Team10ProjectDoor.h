// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GP2Team10ProjectDoor.generated.h"

UCLASS()
class GP2TEAM10PROJECT_API AGP2Team10ProjectDoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGP2Team10ProjectDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Pressure Pad")
		class AGP2Team10ProjectPressurePad* MyPressurePad;

	UPROPERTY(EditAnywhere, Category = "Door")
		bool bDoReset = true;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
		void OnPressurePadActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "Door")
		void OnPressurePadDeactivated();

	UFUNCTION(BlueprintCallable, Category = "Door")
		void ToggleDoorState(bool bPressurePadDown);

private:
};
