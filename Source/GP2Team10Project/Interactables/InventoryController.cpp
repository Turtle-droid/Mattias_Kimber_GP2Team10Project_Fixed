#include "InventoryController.h"
#include "InventoryGameState.h"
//#include "InventoryCharacter.h"
#include "Player/GP2Team10ProjectPlayerCharacter.h"
AInventoryController::AInventoryController()
{
	InventorySlotLimit = 10;
	InventoryWeightLimit = 100;
}

int32 AInventoryController::GetInventoryWeight()
{
	int32 Weight = 0;
	for (auto& Item : Inventory)
	{
		Weight += Item.Weight;
	}

	return Weight;
}

bool AInventoryController::AddItemToInventoryByID(FName ID)
{
	AInventoryGameState* GameState = Cast<AInventoryGameState>(GetWorld()->GetGameState());
	UDataTable* ItemTable = GameState->GetItemDB();
	FInventoryItem* ItemToAdd = ItemTable->FindRow<FInventoryItem>(ID, "");


	if (ItemToAdd)
	{
		if (Inventory.Num() < InventorySlotLimit && GetInventoryWeight() + ItemToAdd->Weight <= InventoryWeightLimit)
		{
			Inventory.Add(*ItemToAdd);
			ReloadInventory();
			return true;
		}
	}
	return false;
}

void AInventoryController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AInventoryController::Interact);
}

void AInventoryController::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
	}
}