#include "GP2Team10ProjectPressurePad.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/GP2Team10ProjectPlayerCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

// Sets default values
AGP2Team10ProjectPressurePad::AGP2Team10ProjectPressurePad()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PressurePadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PressurePadMesh"));
	RootComponent = PressurePadMesh;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->InitBoxExtent(FVector(128.f, 128.f, 128.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGP2Team10ProjectPressurePad::BeginPlay()
{
	Super::BeginPlay();
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGP2Team10ProjectPressurePad::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGP2Team10ProjectPressurePad::OnOverlapEnd);
}

// Notify that the pressure pad has been activated
void AGP2Team10ProjectPressurePad::RunActivationProcedure()
{
	if (!bPressurePadActivated)
	{
		bPressurePadActivated = true;
		OnPressurePadActivated();
		PressurePadState.Broadcast(bPressurePadActivated);

		FMODSoundComponent = UFMODBlueprintStatics::PlayEventAttached(PressurePadActivationSound, RootComponent, NAME_None, RootComponent->GetComponentLocation(), EAttachLocation::KeepWorldPosition, true, true, true);
	}
}

// Check overlapping components to determine if they belong to the player or have sufficient mass to activate pressure pad.
bool AGP2Team10ProjectPressurePad::CheckOverlappingComponents()
{
	bool bBeingPressedDown = false;

	TArray<UPrimitiveComponent*> OverlappingComponents;
	TriggerBox->GetOverlappingComponents(OverlappingComponents);

	if (OverlappingComponents.Num() > 0)
	{
		for (auto& Component : OverlappingComponents)
		{
			// Todo: Found during review, rewrote for clearer readability with new functions for describing the various parts clearly.
			if (CheckForSimulatingMesh(Component) || CheckForPlayer(Component))
			{
				bBeingPressedDown = true;
				RunActivationProcedure();
				break;
			}
		}
	}
	return bBeingPressedDown;
}

bool AGP2Team10ProjectPressurePad::CheckForSimulatingMesh(UPrimitiveComponent* Component)
{
	if (Component->IsA<UStaticMeshComponent>() && Component->IsSimulatingPhysics() && Component->GetMass() >= PressurePadLimit)
		return true;

	else
		return false;
}

bool AGP2Team10ProjectPressurePad::CheckForPlayer(UPrimitiveComponent* Component)
{
	if (Component->IsA<UCapsuleComponent>() && Component->GetOwner()->GetClass()->IsChildOf<AGP2Team10ProjectPlayerCharacter>())
		return true;

	else
		return false;
}

//Check the overlapping stuff on... overlap
void AGP2Team10ProjectPressurePad::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckOverlappingComponents();
}

// When overlap ends make sure it's clear before deactivating
void AGP2Team10ProjectPressurePad::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!CheckOverlappingComponents())
	{
		bPressurePadActivated = false;
		OnPressurePadDeactivated();
		PressurePadState.Broadcast(bPressurePadActivated);

		FMODSoundComponent = UFMODBlueprintStatics::PlayEventAttached(PressurePadDeactivationSound, RootComponent, NAME_None, RootComponent->GetComponentLocation(), EAttachLocation::KeepWorldPosition, true, true, true);
	}
}
