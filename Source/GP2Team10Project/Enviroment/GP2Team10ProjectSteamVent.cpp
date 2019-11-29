#include "GP2Team10ProjectSteamVent.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/SceneComponent.h>
#include "Player/GP2Team10ProjectPlayerCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "Enemies/FGEnemy.h"
#include "GP2Team10ProjectPressurePad.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

// Sets default values
AGP2Team10ProjectSteamVent::AGP2Team10ProjectSteamVent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// TODO: Found inconsistency while reviewing code, changed PressurePadMesh to SteamVentMesh to align with gameobject name for better consistency/clarity.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	SteamPoint = CreateDefaultSubobject<USceneComponent>(TEXT("SteamPoint"));
	SteamPoint->SetupAttachment(RootComponent);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	TriggerBox->InitBoxExtent(FVector(128.f, 128.f, 128.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGP2Team10ProjectSteamVent::BeginPlay()
{
	Super::BeginPlay();

	// TODO: Found inconsistency while reviewing code, removal of OnComponentBeginOverlap since function was unused and therefore unnecessary 
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AGP2Team10ProjectSteamVent::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AGP2Team10ProjectSteamVent::OnOverlapEnd);

	if (bSteamVentActive)
		FMODSoundComponent = UFMODBlueprintStatics::PlayEventAttached(SteamVentSound, RootComponent, NAME_None, RootComponent->GetComponentLocation(), EAttachLocation::KeepWorldPosition, true, true, true);

	if (MyPressurePad)
	{
		MyPressurePad->PressurePadState.AddDynamic(this, &AGP2Team10ProjectSteamVent::ToggleSteamVentState);
	}
}

void AGP2Team10ProjectSteamVent::OnLiftActor()
{
	if (PlayerActor)
	{
		bool bIsGliding = Cast<AGP2Team10ProjectPlayerCharacter>(PlayerActor)->GetGliderStatus();

		if (bIsGliding)
		{
			Cast<AGP2Team10ProjectPlayerCharacter>(PlayerActor)->LaunchCharacter(FVector(0, 0, AirPower), false, true);
		}
	}
}

void AGP2Team10ProjectSteamVent::RemoveUpliftForce()
{
	if (PlayerActor)
	{
		float GliderDownwardForce = Cast<AGP2Team10ProjectPlayerCharacter>(PlayerActor)->GetGliderDownwardForce();

		Cast<AGP2Team10ProjectPlayerCharacter>(PlayerActor)->LaunchCharacter(FVector(0, 0, -GliderDownwardForce), false, true);

		PlayerActor = nullptr;
	}
}

// TODO: Found inconsistency while reviewing code, removal of OnComponentBeginOverlap since function was unused and therefore unnecessary 
void AGP2Team10ProjectSteamVent::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bSteamVentActive)
	{

		if (OtherActor->GetClass()->IsChildOf<AGP2Team10ProjectPlayerCharacter>())
		{
			PlayerActor = OtherActor;

			GetWorld()->GetTimerManager().SetTimer(SteamVentLiftTimerHandle, this, &AGP2Team10ProjectSteamVent::OnLiftActor, LiftBoostInterval, true);
		}
	}
}

// Make sure player loses lift power after leaving
void AGP2Team10ProjectSteamVent::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (bSteamVentActive)
	{
		if (OtherActor->IsA<AGP2Team10ProjectPlayerCharacter>())
		{
			GetWorld()->GetTimerManager().SetTimer(SteamVentLiftTimerHandle, this, &AGP2Team10ProjectSteamVent::RemoveUpliftForce, LiftBoostRemainderTime, false);
		}
	}
}

// Toggle the vents active state 
void AGP2Team10ProjectSteamVent::ToggleSteamVentState(bool bPressurePadDown)
{
	if (bPressurePadDown)
	{
		if (FMODSoundComponent != nullptr)
		{
			FMODSoundComponent->Play();
		}

		else if (FMODSoundComponent == nullptr)
		{
			FMODSoundComponent = UFMODBlueprintStatics::PlayEventAttached(SteamVentSound, RootComponent, NAME_None, RootComponent->GetComponentLocation(), EAttachLocation::KeepWorldPosition, true, true, true);
		}

		bSteamVentActive = true;
		// TODO: Naming inconsistency found while reviewing code, changed function name to match object for consistency/clarity
		OnSteamVentActivated();
	}

	else
	{
		if (bDoReset)
		{
			if (FMODSoundComponent != nullptr)
			{
				if (FMODSoundComponent->IsPlaying())
					FMODSoundComponent->Stop();
			}

			bSteamVentActive = false;
			// TODO: Naming inconsistency found while reviewing code, changed function name to match object for consistency/clarity
			OnSteamVentDeactivated();
		}
	}
}