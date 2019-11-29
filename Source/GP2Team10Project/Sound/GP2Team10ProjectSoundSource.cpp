// Fill out your copyright notice in the Description page of Project Settings.

#include "GP2Team10ProjectSoundSource.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "GP2Team10ProjectSoundTrigger.h"

// Sets default values
AGP2Team10ProjectSoundSource::AGP2Team10ProjectSoundSource()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGP2Team10ProjectSoundSource::PlaySoundAtLocation()
{
	MyEventInstance = UFMODBlueprintStatics::PlayEventAtLocation(GetWorld(), MySoundEvent, GetActorTransform(), bAutoPlaySound);
}

// Called when the game starts or when spawned
void AGP2Team10ProjectSoundSource::BeginPlay()
{
	Super::BeginPlay();
	PlaySoundAtLocation();
}

