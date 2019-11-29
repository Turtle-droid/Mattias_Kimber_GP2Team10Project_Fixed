#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FMODBlueprintStatics.h"
#include "GP2Team10ProjectSoundSource.generated.h"

UCLASS()
class GP2TEAM10PROJECT_API AGP2Team10ProjectSoundSource : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGP2Team10ProjectSoundSource();	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
		class UFMODEvent* MySoundEvent;

	FFMODEventInstance MyEventInstance;

	UFUNCTION()
		void PlaySoundAtLocation();

	bool bAutoPlaySound = true;

private:

	virtual void BeginPlay() override;
};
