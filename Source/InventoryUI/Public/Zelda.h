#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Zelda.generated.h"

class UInventoryComponent;


UCLASS()
class INVENTORYUI_API AZelda : public ACharacter
{
	GENERATED_BODY()

public:
	AZelda();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UInventoryComponent* InventoryComponent;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateInventory(struct FAllItem AllItem);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
		class UCameraComponent* Camera;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Interact();

	void OpenInventory();

	UPROPERTY(EditAnywhere, Category = Input)
		class UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
		class UInputAction* OpenInventoryAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InteractAction;


private:


};
