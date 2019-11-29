#include "InventoryItem.h"

FInventoryItem::FInventoryItem()
{
	this->Name = FText::FromString("No Name");
	this->Consumeable = false;
	this->Weight = 0;
	this->Value = 0;
	this->Description = FText::FromString("");
}