#include "ManualPickup.h"
#include "InventoryController.h"

class AGP2Team10ProjectPlayerCharacter;

AManualPickup::AManualPickup()
{	
	//Inventory properties
	ItemID = FName("No ID");
	Super::Name = "InsertItemName";
	Super::Action = "InsertItemAction";
}

void AManualPickup::Interact_Implementation(APlayerController* Controller)
{
	Super::Interact_Implementation(Controller);
	AInventoryController* IController = Cast<AInventoryController>(Controller);
	if (IController->AddItemToInventoryByID(ItemID))
		Destroy();
}

void AManualPickup::HandlePickupEnterRange(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AGP2Team10ProjectPlayerCharacter>())
	{
		Target = Cast<AGP2Team10ProjectPlayerCharacter>(OtherActor);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("In Pickup Range"));
	}
}
void AManualPickup::HandlePickupEndRange(UPrimitiveComponent* OverLappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (OtherActor == Target)
	{
		Target = nullptr;
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Out of pickup range"));
	}
}
