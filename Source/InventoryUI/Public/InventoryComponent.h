#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include "InventoryComponent.generated.h"

struct FItemInfo;

USTRUCT(BlueprintType)
struct FAllItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FItemStruct> Sword;
	
	UPROPERTY(EditAnywhere)
	TArray<FItemStruct> Shield;
	
	UPROPERTY(EditAnywhere)
	TArray<FItemStruct> Eatables;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYUI_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int64 Money = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth = 100.f;

	UFUNCTION(BlueprintCallable)
	void IncreaseHealth(float AddToHealth, float AddToMaxHealth);

	UFUNCTION(BlueprintCallable)
	void DecreaseHealth(float SubToHealth, float SubToMaxHealth);

	FORCEINLINE int64 GetMoney() { return Money; }

	FORCEINLINE void SetMoney(int64 NewMoney) { Money = NewMoney; }

	bool AddToInventory(AItem* ItemToAdd);

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<class AItem> PrevItem;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<AItem> ThisItem = nullptr;

	UPROPERTY(EditAnywhere)
		FAllItem AllItems;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
		bool bShowTraceDebug = false;


public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
