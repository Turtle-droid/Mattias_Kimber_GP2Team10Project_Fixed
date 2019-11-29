#include "GP2Team10ProjectAmbienceMaster.h"
#include "FMODEvent.h"
#include "GP2Team10ProjectSoundTrigger.h"

// Sets default values
AGP2Team10ProjectAmbienceMaster::AGP2Team10ProjectAmbienceMaster()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGP2Team10ProjectAmbienceMaster::OnIndoorTrigger(bool bIsIndoors)
{
	if (bIsIndoors)
	WindAmbienceInstance.Instance->setParameterByName(FmodParameterName, FmodParameterMax);

	else 
		WindAmbienceInstance.Instance->setParameterByName(FmodParameterName, FmodParameterMin);
}

// Called when the game starts or when spawned
void AGP2Team10ProjectAmbienceMaster::BeginPlay()
{
	Super::BeginPlay();


	// Todo: Found while reviewing code, moved ambiance play to their own function for readability/abstraction.
	PlayAmbiences();
	
	if (MySoundTriggers.Num() > 0)
	{
		for (int i = 0; i < MySoundTriggers.Num(); i++)
		{
			MySoundTriggers[i]->SoundTriggerDelegate.AddDynamic(this, &AGP2Team10ProjectAmbienceMaster::OnIndoorTrigger);
		}
	}
}

// Todo: Found while reviewing code, moved ambiance play to their own function for readability/abstraction.
void AGP2Team10ProjectAmbienceMaster::PlayAmbiences()
{
	if (WindAmbience)
		WindAmbienceInstance = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), WindAmbience, true);

	if (MusicAmbience)
		MusicAmbienceInstance = UFMODBlueprintStatics::PlayEvent2D(GetWorld(), MusicAmbience, true);
}


