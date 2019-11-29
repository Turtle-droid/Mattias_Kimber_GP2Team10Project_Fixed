#include "GP2Team10ProjectPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/GP2Team10ProjectHealthComponent.h"
#include "Components/TimelineComponent.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
//Todo:Review after removing non essential parts, Removed unnecessary includes for cleanliness/optimization

//////////////////////////////////////////////////////////////////////////
// AGP2_Team10Character

// Todo: Reviewed code and moved stuff around and added regions for easier navigation of code.
#pragma region CharacterSetup

AGP2Team10ProjectPlayerCharacter::AGP2Team10ProjectPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	MyCapsule = GetCapsuleComponent();
	MyCapsule->OnComponentHit.AddDynamic(this, &AGP2Team10ProjectPlayerCharacter::OnHit);

	// set our turn rates for input
	BaseTurnRate = 60.f;
	BaseLookUpRate = 60.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = JumpVelocity;
	GetCharacterMovement()->AirControl = DefaultAirControl;
	GetCharacterMovement()->GravityScale = DefaultGravityScale;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 200.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = false;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Remember the default walking speed
	DefaultMaxWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
}


//////////////////////////////////////////////////////////////////////////
// Input

void AGP2Team10ProjectPlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Thrust", IE_Pressed, this, &AGP2Team10ProjectPlayerCharacter::AimThrust);
	PlayerInputComponent->BindAction("Thrust", IE_Released, this, &AGP2Team10ProjectPlayerCharacter::Thrust);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &AGP2Team10ProjectPlayerCharacter::HandleDashStart);
	PlayerInputComponent->BindAction("Dash", IE_Released, this, &AGP2Team10ProjectPlayerCharacter::HandleDashStop);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGP2Team10ProjectPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGP2Team10ProjectPlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &AGP2Team10ProjectPlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGP2Team10ProjectPlayerCharacter::TurnAtRate);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGP2Team10ProjectPlayerCharacter::LookUpAtRate);

	//Functions used to quickly lock and unlock character abilities for testing
	PlayerInputComponent->BindAction("Reset", IE_Pressed, this, &AGP2Team10ProjectPlayerCharacter::ResetCharacter);
	PlayerInputComponent->BindAction("Unlock", IE_Pressed, this, &AGP2Team10ProjectPlayerCharacter::UnlockAbilities);
}

void AGP2Team10ProjectPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	if (World)
	{
		CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	}

	MyHealthComponent = this->FindComponentByClass<UGP2Team10ProjectHealthComponent>();

	if (MyHealthComponent)
	{
		//Reset character on death
		MyHealthComponent->OnHealthZero.AddDynamic(this, &AGP2Team10ProjectPlayerCharacter::ResetCharacter);
	}

	if (CurveFloat)
	{
		FOnTimelineFloat ProgressFunction;

		ProgressFunction.BindUFunction(this, FName("HandleTimeProgress"));

		//Setting up the loop status and the function that is going to fire when the timeline ticks
		MyTimeline.AddInterpFloat(CurveFloat, ProgressFunction);
		MyTimeline.SetLooping(false);
	}

	//Initial Respawn point
	CurrentRespawnLocation = GetActorLocation();
	GetWorldTimerManager().SetTimer(CheckCurrentLocationTimer, this, &AGP2Team10ProjectPlayerCharacter::CheckCurrentLocationHeight, 0.1f, true);
}

#pragma endregion

#pragma region Movement/Update

void AGP2Team10ProjectPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MyTimeline.TickTimeline(DeltaTime);
}

//Turn character using controller
void AGP2Team10ProjectPlayerCharacter::TurnAtRate(float Rate)
{
	// Bosted rate due to slowdown affecting camera
	if (bIsSlowmotion)
		AddControllerYawInput(Rate * BaseTurnRate * 5 * GetWorld()->GetDeltaSeconds());

	else
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AGP2Team10ProjectPlayerCharacter::AddControllerYawInput(float Rate)
{
	Super::AddControllerYawInput(Rate);
}

void AGP2Team10ProjectPlayerCharacter::LookUpAtRate(float Rate)
{
	// Bosted rate due to slowdown affecting camera
	if (bIsSlowmotion)
		AddControllerPitchInput(Rate * BaseTurnRate * 5 * GetWorld()->GetDeltaSeconds());

	else
		AddControllerPitchInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGP2Team10ProjectPlayerCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGP2Team10ProjectPlayerCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//Jump / glider  function if held down
void AGP2Team10ProjectPlayerCharacter::Jump()
{
	Super::Jump();

	bJumpPressed = true;

	if (bIsGrounded)
	{
		GetCharacterMovement()->bNotifyApex = true;
	}

	else if (GetCharacterMovement()->IsFalling() && !bIsGliding)
	{
		UFMODBlueprintStatics::PlayEvent2D(World, TakeOffSound, true);
		DeployGlider();
	}

	else if (bIsGliding)
	{
		StopJumping();
	}
}

//Deploy glider after reaching jump apex
void AGP2Team10ProjectPlayerCharacter::NotifyJumpApex()
{
	if (!bIsThrusting && !bIsGliding && bJumpPressed)
	{
		UFMODBlueprintStatics::PlayEvent2D(World, TakeOffSound, true);
		DeployGlider();
	}
}

//Stop jumping / gliding 
void AGP2Team10ProjectPlayerCharacter::StopJumping()
{
	Super::StopJumping();
	// Jump is no longer pressed 
	bJumpPressed = false;

	if (bIsGliding)
	{
		//Function meant to provide a bit more imediate stop while gliding and releasing the key
		FVector CharacterVelocity = this->GetVelocity();
		LaunchCharacter(CharacterVelocity * GlidingSlowdownScale, true, true);
	}
	StopGlide();
}

#pragma region GliderFunctions

//Function to check if glider is allowed to be deployed and if so starts the gliding function after a small delay
void AGP2Team10ProjectPlayerCharacter::DeployGlider()
{
	if (bHasGlider && !bIsGliding)
	{
		FHitResult OutHit;
		FVector Start = GetCapsuleComponent()->GetComponentLocation();
		FVector End = FVector(Start.X, Start.Y, Start.Z - MinHeight);
		FCollisionQueryParams CollisionParams;

		//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);

		if (!bIsThrusting)
		{
			if (!GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
			{
				bIsGrounded = false;
				GetWorldTimerManager().SetTimer(GliderTimer, this, &AGP2Team10ProjectPlayerCharacter::Glide, 0.1f, false);
			}
		}
	}
}

// Lets the character glide by changing the gravity scale and allows character to move faster
void AGP2Team10ProjectPlayerCharacter::Glide()
{
	bIsGliding = true;
	ApplyDownwardForce();
	GetCharacterMovement()->MaxWalkSpeed = GlidingSpeed;
	GetCharacterMovement()->GravityScale = GlidingGravityScale;
	GetCharacterMovement()->AirControl = GlidingAirControl;
}

// Stops the glide by reseting values
void AGP2Team10ProjectPlayerCharacter::StopGlide()
{
	bIsGliding = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkingSpeed;
	GetCharacterMovement()->AirControl = DefaultAirControl;
	GetCharacterMovement()->GravityScale = DefaultGravityScale;

	//Set minheight requiered for glider take off
	MinHeight = DefaultMinHeight;
}

//Function for aiming the thrust by slowing down the game world
void AGP2Team10ProjectPlayerCharacter::AimThrust()
{
	//Only allowed in the air and with thrust charges
	if (GetCharacterMovement()->IsFalling() && ThrustCharges > 0)
	{
		bIsSlowmotion = true;
		bThrustCharged = true;
		// Player slowmo sound
		UFMODBlueprintStatics::PlayEvent2D(World, SlowmoSound, true);
		//Use timeline for slowdown effect
		MyTimeline.PlayFromStart();
		//StopGlide();
	}
}

//Let the character dash through the air using launch character
void AGP2Team10ProjectPlayerCharacter::Thrust()
{
	if (GetCharacterMovement()->IsFalling() && bThrustCharged)
	{
		bThrustCharged = false;
		bIsThrusting = true;

		OnThrust();

		//Play dash sound
		UFMODBlueprintStatics::PlayEvent2D(World, AirDashSound, true);

		//Camera is used as direction for thrust
		LaunchVector = FollowCamera->GetForwardVector() * ThrustPower;

		StopGlide();

		LaunchCharacter(LaunchVector, true, true);
		ThrustCharges -= 1;

		//Timer before slowdown of thrust is applied
		GetWorldTimerManager().SetTimer(ThrustTimer, this, &AGP2Team10ProjectPlayerCharacter::SlowThrust, ThrustDuration, false);
	}
}

//Function the slows thrust by lowering scaling
void AGP2Team10ProjectPlayerCharacter::SlowThrust()
{
	bIsThrusting = false;
	StopGlide();
	LaunchCharacter(LaunchVector * ThrustSlowingScale, true, true);

	//Timer for when to attempt glider reapplication 
	GetWorldTimerManager().SetTimer(GliderTimer, this, &AGP2Team10ProjectPlayerCharacter::ReApplyGlider, 0.2f, false);
}

//If the jump button is held attempt to deploy the glider again
void AGP2Team10ProjectPlayerCharacter::ReApplyGlider()
{
	if (bJumpPressed)
		DeployGlider();
}

//Sets global time dilation based on timeline
void AGP2Team10ProjectPlayerCharacter::HandleTimeProgress(float Value)
{
	//Setting up the new location of our actor
	UGameplayStatics::SetGlobalTimeDilation(World, Value);
}

//Reset global time dilation 
void AGP2Team10ProjectPlayerCharacter::ResetTime()
{
	bIsSlowmotion = false;

	//Put timeline back to start
	MyTimeline.Stop();
	MyTimeline.SetNewTime(0.0f);
	UGameplayStatics::SetGlobalTimeDilation(World, DefaultTimeDialation);
}

//Landing function that reset player thrustcharges and movement to be ground based
void AGP2Team10ProjectPlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bIsGrounded = true;
	bJumpPressed = false;

	//Play landing sound
	UFMODBlueprintStatics::PlayEvent2D(World, LandingSound, true);

	// Stop gliding and make sure time is reset
	StopGlide();
	ResetTime();

	//Refill thurstcharges to max
	ThrustCharges = MaxThrustCharges;
}

// Function for receiving Dash button input
void AGP2Team10ProjectPlayerCharacter::HandleDashStart()
{
	//If the character is falling call aimthrust
	if (GetCharacterMovement()->IsFalling())
	{
		AimThrust();
	}
}

// Function for receiving Dash button release which resets time and starts thrust
void AGP2Team10ProjectPlayerCharacter::HandleDashStop()
{
	ResetTime();
	if (GetCharacterMovement()->IsFalling())
	{
		Thrust();
	}
}

void AGP2Team10ProjectPlayerCharacter::SetDownwardTimer(float Time)
{
	GetWorldTimerManager().SetTimer(DownwardTimer, this, &AGP2Team10ProjectPlayerCharacter::ApplyDownwardForce, 0.5f, false);
}

void AGP2Team10ProjectPlayerCharacter::ApplyDownwardForce()
{
	//Force the character downward when gliding
	LaunchCharacter(FVector(0, 0, -GliderDownwardForce), false, true);
}

#pragma endregion

#pragma region CharacterAbilityUnlockers
// Allow easy acess to abilites for testing
void AGP2Team10ProjectPlayerCharacter::UnlockAbilities()
{
	this->JumpMaxCount = 1;
	MaxThrustCharges = 1;
	bHasGlider = true;
}

//Also lock abilites 
void AGP2Team10ProjectPlayerCharacter::LockAbilities()
{
	this->JumpMaxCount = 0;
	MaxThrustCharges = 0;
	bHasGlider = false;
}

#pragma endregion

#pragma region CharacterReset/Respawn
//Blueprint callable function to change respawn position
void AGP2Team10ProjectPlayerCharacter::ChangeCurrentRespawnPoint(FVector Location, FRotator Rotation)
{
	CurrentRespawnRotation = Rotation;
	CurrentRespawnLocation = Location;
}

//Todo: Found during profiling, separated from tick function for optimization using timer and clarity. 
void AGP2Team10ProjectPlayerCharacter::CheckCurrentLocationHeight()
{
	//Keep track of current and highest reached altitude
	CurrentAltitude = GetActorLocation().Z;

	//Set new highest altitude if our currentaltitude is higher
	if (CurrentAltitude > HighestReachedAltitude)
	{
		HighestReachedAltitude = CurrentAltitude;
	}

	// If the player falls below a certain threshold of their highest altitude they can reset to checkpoint
	if (HighestReachedAltitude - CurrentAltitude >= AltitudeDifferenceLimit)
	{
		bAllowReset = true;
	}
}

//Reset character to last checkpoint with health and charges restored
void AGP2Team10ProjectPlayerCharacter::ResetCharacter()
{
	if (bAllowReset == true)
	{
		bAllowReset = false;

		StopGlide();
		ResetTime();
		ThrustCharges = MaxThrustCharges;

		this->TeleportTo(CurrentRespawnLocation, CurrentRespawnRotation, false, false);

		if (MyHealthComponent)
		{
			MyHealthComponent->ModifyHealth(MyHealthComponent->GetMaxHealth());
		}
	}
}

#pragma endregion

#pragma region HitFunction

//Character hit function causing knock back on gliding into things at certain speeds
void AGP2Team10ProjectPlayerCharacter::OnHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Todo found during review, removed unnecessary step variable for optimization and shorter block of code
	float VelocityLength;

	VelocityLength = this->GetVelocity().Size();

	if (bIsGliding)
	{
		if (VelocityLength >= GliderImpactLimit)
		{
			FVector PushbackVector = (FVector(Hit.ImpactNormal.X, Hit.ImpactNormal.Y, 0));

			LaunchCharacter(PushbackVector * 500, true, false);

			StopGlide();
		}
	}
}

#pragma endregion