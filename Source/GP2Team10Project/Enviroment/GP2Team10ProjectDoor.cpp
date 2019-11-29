// Fill out your copyright notice in the Description page of Project Settings.
#include "GP2Team10ProjectDoor.h"
#include "FMODBlueprintStatics.h"
#include "GP2Team10ProjectPressurePad.h"

// Sets default values
AGP2Team10ProjectDoor::AGP2Team10ProjectDoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGP2Team10ProjectDoor::BeginPlay()
{
	Super::BeginPlay();

	if (MyPressurePad)
	{
		MyPressurePad->PressurePadState.AddDynamic(this, &AGP2Team10ProjectDoor::ToggleDoorState);
	}
}

void AGP2Team10ProjectDoor::ToggleDoorState(bool bPressurePadDown)
{
	if (bPressurePadDown)
	{
		OnPressurePadActivated();

		//Todo: found during review, removed unused audio event code.
	}

	else
	{
		if (bDoReset)
		{
			OnPressurePadDeactivated();
		}
	}
}

