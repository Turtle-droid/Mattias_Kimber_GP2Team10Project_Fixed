// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMODBlueprintStatics.h"
#include "GP2Team10ProjectAmbienceMaster.generated.h"

UCLASS()
class GP2TEAM10PROJECT_API AGP2Team10ProjectAmbienceMaster : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGP2Team10ProjectAmbienceMaster();

protected:
	/** Test audio event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODEvent* WindAmbience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODEvent* MusicAmbience;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		 FFMODEventInstance MusicAmbienceInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		 FFMODEventInstance WindAmbienceInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		TArray<class AGP2Team10ProjectSoundTrigger*> MySoundTriggers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		int FmodParameterMax = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		int FmodParameterMin = 0;

	UFUNCTION(Category = "AmbienceMaster")
		void OnIndoorTrigger(bool bIsIndoors);

private:
	virtual void BeginPlay() override;

	void PlayAmbiences();

	const char* FmodParameterName = "Indoors";
};
