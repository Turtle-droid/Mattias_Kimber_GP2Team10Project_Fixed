#include "Interactable.h"

AInteractable::AInteractable()
{
	Name = "Interactable";
	Action = "Interact";
}

void AInteractable::Interact_Implementation(APlayerController* Controller)
{
	return;
}

FString AInteractable::GetInteractText() const
{
	return FString::Printf(TEXT("%s: Press E to %s"), *Name, *Action);
}

