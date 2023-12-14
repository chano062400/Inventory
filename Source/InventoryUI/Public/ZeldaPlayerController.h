#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZeldaPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYUI_API AZeldaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	void OpenInventory();

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UUserWidget> InventoryWidget;
	

};
