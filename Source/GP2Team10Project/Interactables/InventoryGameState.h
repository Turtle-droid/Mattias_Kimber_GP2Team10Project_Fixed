#pragma once

#include "Engine.h"
#include "GameFramework/GameStateBase.h"
#include "InventoryGameState.generated.h"

UCLASS()
class GP2TEAM10PROJECT_API AInventoryGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AInventoryGameState();

	UDataTable* GetItemDB() const;

protected:
	UPROPERTY(EditDefaultsOnly)
		class UDataTable* ItemDB;
};