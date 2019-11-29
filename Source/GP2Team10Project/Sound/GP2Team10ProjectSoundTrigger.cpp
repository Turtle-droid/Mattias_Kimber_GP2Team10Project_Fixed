// Fill out your copyright notice in the Description page of Project Settings.

#include "GP2Team10ProjectSoundTrigger.h"
#include "Components/BoxComponent.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

// Sets default values
AGP2Team10ProjectSoundTrigger::AGP2Team10ProjectSoundTrigger()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->InitBoxExtent(FVector(128.f, 128.f, 128.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerBox;
}

// Called when the game starts or when spawned
void AGP2Team10ProjectSoundTrigger::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGP2Team10ProjectSoundTrigger::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGP2Team10ProjectSoundTrigger::OnOverlapEnd);
}

void AGP2Team10ProjectSoundTrigger::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SoundTriggerDelegate.Broadcast(true);
}

void AGP2Team10ProjectSoundTrigger::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	SoundTriggerDelegate.Broadcast(false);
}
