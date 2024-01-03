#include "ZeldaPlayerController.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "Zelda.h"
#include "InventoryComponent.h"

void AZeldaPlayerController::BeginPlay()
{
	Inventory = CreateWidget<UUserWidget>(this, InventoryWidget);
	check(Inventory);
}

void AZeldaPlayerController::OpenInventory()
{
	Inventory->AddToViewport();
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(Inventory->TakeWidget());
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}
