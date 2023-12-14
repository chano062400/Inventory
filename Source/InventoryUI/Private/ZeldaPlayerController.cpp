#include "ZeldaPlayerController.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"

void AZeldaPlayerController::BeginPlay()
{

}

void AZeldaPlayerController::OpenInventory()
{
	UUserWidget* Inventory = CreateWidget<UUserWidget>(this, InventoryWidget);
	check(Inventory);

	Inventory->AddToViewport();
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(Inventory->TakeWidget());
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}
