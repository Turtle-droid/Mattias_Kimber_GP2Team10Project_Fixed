
#pragma once

//#include "CoreMinimal.h" // Changed CoreMinimal to Engine 
#include "Engine.h"
#include "GameFramework/Character.h"
#include "GP2Team10ProjectPlayerCharacter.generated.h"



UCLASS(config = Game)
class AGP2Team10ProjectPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGP2Team10ProjectPlayerCharacter();

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE bool GetGliderStatus() const { return bIsGliding; }

	FORCEINLINE float GetGliderDownwardForce() const { return GliderDownwardForce; }

	UFUNCTION(BlueprintCallable, Category = Respawn)
		void ChangeCurrentRespawnPoint(FVector Location, FRotator Rotation);

	void CheckCurrentLocationHeight();

	UFUNCTION(BlueprintCallable)
		void SetDownwardTimer(float Time);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Checkpoints)
		FVector CurrentRespawnLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Glider)
		bool bIsGliding = false;

protected:

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float JumpVelocity = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float DefaultGravityScale = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float GlidingGravityScale = 0.05f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float GlidingSpeed = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float GliderDownwardForce = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float DefaultAirControl = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float GlidingAirControl = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float GlidingSlowdownScale = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float ThrustDuration = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float ThrustSlowingScale = 0.2f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float GliderImpactLimit = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Glider)
		float ThrustPower = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float SlowdownScale = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = Glider)
		float DefaultMinHeight = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Glider)
		int MaxThrustCharges = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Glider)
		int  ThrustCharges = 1;

	/*Curve float reference*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		UCurveFloat* CurveFloat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Checkpoints)
		FRotator CurrentRespawnRotation;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Glider)
		bool bHasGlider = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Glider)
		float MinHeight = 200;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Glider)
		float CurrentAltitude;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Glider)
		float HighestReachedAltitude;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float AltitudeDifferenceLimit = 10000;

	float DefaultMaxWalkingSpeed;

	float DefaultTimeDialation = 1.0f;

	UCapsuleComponent* MyCapsule;

	APlayerCameraManager* CameraManager;

	UWorld* World;

	FTimeline MyTimeline;

	/** Test audio event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODEvent* SlowmoSound;

	/** Test audio event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		UFMODEvent* AirDashSound;

	/** Test audio event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		UFMODEvent* TakeOffSound;

	/** Test audio event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		UFMODEvent* LandingSound;

	/** Test audio event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		UFMODEvent* ImpactSound;

	UFUNCTION()
		void HandleTimeProgress(float Value);

	UFUNCTION()
		void ResetTime();

	UPROPERTY(EditDefaultsOnly, Category = HealthComponent)
		class	UGP2Team10ProjectHealthComponent*  MyHealthComponent;

	UFUNCTION()
		void ResetCharacter();

	UFUNCTION()
		void OnHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintImplementableEvent)
		void OnThrust();

private:

	bool bIsThrusting = false;

	bool bThrustCharged = false;

	bool bJumpPressed = false;

	bool bIsGrounded = true;

	bool bIsSlowmotion = false;

	bool bAllowReset = false;

	FVector LaunchVector;

	FTimerHandle GliderTimer;

	FTimerHandle ThrustTimer;

	FTimerHandle DownwardTimer;

	FTimerHandle CheckCurrentLocationTimer;

	virtual void Landed(const FHitResult& Hit) override;

	void NotifyJumpApex() override;

	//Start glide function
	void DeployGlider();

	//Paraglide function
	void Glide();

	//Paraglide end function
	void StopGlide();

	//Thrust aim function
	void AimThrust();

	//Thrust function
	void Thrust();

	void SlowThrust();

	void ReApplyGlider();

	void HandleDashStart();

	void HandleDashStop();

	//Start glide function
	void ApplyDownwardForce();

	void Jump() override;

	void StopJumping() override;

	UFUNCTION(BlueprintCallable)
		void UnlockAbilities();

	UFUNCTION(BlueprintCallable)
		void LockAbilities();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	virtual void AddControllerYawInput(float Rate) override;

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

	virtual void BeginPlay() override;

	void Tick(float DeltaTime) override;
};
