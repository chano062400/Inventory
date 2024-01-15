#include "InventoryComponent.h"
#include "Money.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Item.h"
#include "Zelda.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UInventoryComponent::IncreaseHealth(float AddToHealth, float AddToMaxHealth)
{
	MaxHealth += AddToMaxHealth;
	Health = FMath::Clamp(Health + AddToHealth, 0, MaxHealth);
	OnHealthChanged.Broadcast();
}

void UInventoryComponent::DecreaseHealth(float SubToHealth, float SubToMaxHealth)
{
	MaxHealth -= SubToMaxHealth;
	Health = FMath::Clamp(Health - SubToHealth, 0 , MaxHealth);
	OnHealthChanged.Broadcast();
}

bool UInventoryComponent::AddToInventory(AItem* ItemToAdd)
{
	FDataTableRowHandle ItemTableHandle = ItemToAdd->ItemStruct.ThisItemTableHandle;

	if (ItemToAdd->ItemStruct.ItemTypes == EItemTypes::EIT_Sword)
	{
		int32 LimitSize = AllItems.Sword.Num();
		int CanAddIdx = LimitSize - 1; // 새로 추가할 수 있는 위치 
		bool IsInInventoryThisItem = false; 
		for (int idx = 0; idx < AllItems.Sword.Num(); idx++)
		{
			auto& Item = AllItems.Sword[idx];
			if(Item.ItemTypes == EItemTypes::EIT_None) CanAddIdx = FMath::Min(CanAddIdx, idx); 

			if (CanAddIdx == 0) // 해당 종류의 인벤토리에 아무것도 없을 경우.
			{
				//AllItems.Sword.Insert(ItemToAdd->ItemStruct, CanAddIdx);
				//AllItems.Sword.SetNum(LimitSize); // Insert시 사이즈가 1개씩 늘어나기 때문에 최대 인벤토리 사이즈로 맞춰줌.
				AllItems.Sword[CanAddIdx] = ItemToAdd->ItemStruct;
				return true;
			}
			
			if (Item.ThisItemTableHandle.RowName == ItemToAdd->ItemStruct.ThisItemTableHandle.RowName)// 해당 아이템이 인벤토리에 이미 있는 경우.
			{
				FName Name = Item.ThisItemTableHandle.RowName; 
				FItemInfo* ItemInfo = Item.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");
				int32 QuantitiyToAdd = Item.Quantity + ItemToAdd->ItemStruct.Quantity; // 추가되고나서의 수량

				if (ItemInfo && Item.Quantity < ItemInfo->StackSize) // 최대 수량보다 적게 갖고있다면
				{
					int32 ItemQuantity = FMath::Clamp(QuantitiyToAdd, 0, ItemInfo->StackSize); // Clamp를 이용해서 가질 수 있는 최대량(stacksize) 이하로 추가.
					Item.Quantity = ItemQuantity;
					IsInInventoryThisItem = true;
					//AllItems.Sword.SetNum(LimitSize);
					return true;
				}
				else // 최대량으로 가지고 있다면 더 이상 가질 수 없음.
				{
					IsInInventoryThisItem = true;
					//To Do (최대 수량을 갖고 있다는 메세지 위젯 출력)
					return false;
				}
			}
		}

		if (!IsInInventoryThisItem && CanAddIdx > 0) // 해당 종류의 인벤토리에 다른 아이템은 있지만 해당 아이템은 없는 경우.
		{
			//AllItems.Sword.Insert(ItemToAdd->ItemStruct, CanAddIdx);
			//AllItems.Sword.SetNum(LimitSize); // Insert시 사이즈가 1개씩 늘어나기 때문에 최대 인벤토리 사이즈로 맞춰줌.
			AllItems.Sword[CanAddIdx] = ItemToAdd->ItemStruct;
			return true;
		}


		return false;
	}

	if (ItemToAdd->ItemStruct.ItemTypes == EItemTypes::EIT_Shield)
	{
		int32 LimitSize = AllItems.Shield.Num();
		int CanAddIdx = LimitSize - 1;
		bool IsInInventoryThisItem = false;
		for (int idx = 0; idx < AllItems.Shield.Num(); idx++)
		{
			auto& Item = AllItems.Shield[idx];
			if (Item.ItemTypes == EItemTypes::EIT_None) CanAddIdx = FMath::Min(CanAddIdx, idx); // 새로 추가할 수 있는 위치 

			if (CanAddIdx == 0) // 해당 종류의 인벤토리에 아무것도 없을 경우.
			{
				/*AllItems.Shield.Insert(ItemToAdd->ItemStruct, CanAddIdx);
				AllItems.Shield.SetNum(LimitSize);*/
				AllItems.Shield[CanAddIdx] = ItemToAdd->ItemStruct;
				return true;
			}

			if (Item.ThisItemTableHandle.RowName == ItemToAdd->ItemStruct.ThisItemTableHandle.RowName)// 해당 아이템이 인벤토리에 이미 있는 경우.
			{
				FName Name = Item.ThisItemTableHandle.RowName;
				FItemInfo* ItemInfo = Item.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");
				int32 QuantitiyToAdd = Item.Quantity + ItemToAdd->ItemStruct.Quantity; // 추가되고 나서의 양 

				if (ItemInfo && Item.Quantity < ItemInfo->StackSize) // 최대 수량보다 적게 갖고있다면
				{
					int32 ItemQuantity = FMath::Clamp(QuantitiyToAdd, 0, ItemInfo->StackSize); // Clamp를 이용해서 가질 수 있는 최대량(stacksize) 이하로 추가.
					Item.Quantity = ItemQuantity;
					IsInInventoryThisItem = true;
					//AllItems.Shield.SetNum(LimitSize);
					return true;
				}
				else // 최대량으로 가지고 있다면 더 이상 가질 수 없음.
				{
					IsInInventoryThisItem = true;
					//To Do (최대 수량을 갖고 있다는 메세지 위젯 출력)
					return false;
				}
			}
		}
		if (!IsInInventoryThisItem && CanAddIdx > 0) // 해당 종류의 인벤토리에 다른 아이템은 있지만 해당 아이템은 없는 경우.
		{
			//AllItems.Shield.Insert(ItemToAdd->ItemStruct, CanAddIdx);
			//AllItems.Shield.SetNum(LimitSize);  // Insert시 사이즈가 1개씩 늘어나기 때문에 최대 인벤토리 사이즈로 맞춰줌.
			AllItems.Shield[CanAddIdx] = ItemToAdd->ItemStruct;
			return true;
		}


		return false;
	}

	if (ItemToAdd->ItemStruct.ItemTypes == EItemTypes::EIT_Eatables)
	{
		int32 LimitSize = AllItems.Eatables.Num();
		int CanAddIdx = LimitSize -1 ;
		bool IsInInventoryThisItem = false;
		for (int idx = 0; idx < AllItems.Eatables.Num(); idx++)
		{
			auto& Item = AllItems.Eatables[idx];
			if (Item.ItemTypes == EItemTypes::EIT_None) CanAddIdx = FMath::Min(CanAddIdx, idx); // 새로 추가할 수 있는 위치 

			if (CanAddIdx == 0) // 해당 종류의 인벤토리에 아무것도 없을 경우.
			{
				//AllItems.Eatables.Insert(ItemToAdd->ItemStruct, CanAddIdx);
				//AllItems.Eatables.SetNum(LimitSize); // Insert시 사이즈가 1개씩 늘어나기 때문에 최대 인벤토리 사이즈로 맞춰줌.
				AllItems.Eatables[CanAddIdx] = ItemToAdd->ItemStruct;
				return true;
			}

			if (Item.ThisItemTableHandle.RowName == ItemToAdd->ItemStruct.ThisItemTableHandle.RowName) // 해당 아이템이 인벤토리에 이미 있는 경우.
			{
				FName Name = Item.ThisItemTableHandle.RowName;
				FItemInfo* ItemInfo = Item.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");
				int32 QuantitiyToAdd = Item.Quantity + ItemToAdd->ItemStruct.Quantity; // 추가되고 나서의 양 

				if (ItemInfo && Item.Quantity < ItemInfo->StackSize) // 최대 수량보다 적게 갖고있다면
				{
					int32 ItemQuantity = FMath::Clamp(QuantitiyToAdd, 0, ItemInfo->StackSize); // Clamp를 이용해서 가질 수 있는 최대량(stacksize) 이하로 추가.
					Item.Quantity = ItemQuantity;
					IsInInventoryThisItem = true;
					/*AllItems.Eatables.SetNum(LimitSize);*/
					return true;
				}
				else // 최대량으로 가지고 있다면 더 이상 가질 수 없음.
				{
					IsInInventoryThisItem = true;
					//To Do (최대 수량을 갖고 있다는 메세지 위젯 출력)
					return false;
				}
			}
		}
		if (!IsInInventoryThisItem && CanAddIdx > 0) // 해당 종류의 인벤토리에 다른 아이템은 있지만 해당 아이템은 없는 경우.
		{
			//AllItems.Eatables.Insert(ItemToAdd->ItemStruct, CanAddIdx);
			//AllItems.Eatables.SetNum(LimitSize); // Insert시 사이즈가 1개씩 늘어나기 때문에 최대 인벤토리 사이즈로 맞춰줌.
			AllItems.Eatables[CanAddIdx] = ItemToAdd->ItemStruct;
			return true;
		}
	}
	
	return false;
}

void UInventoryComponent::IncreaseInventorySize(int AddToSword, int AddToShield, int AddToEatables)
{
	AllItems.Sword.SetNum(AllItems.Sword.Num() + AddToSword);
	AllItems.Shield.SetNum(AllItems.Shield.Num() + AddToShield);
	AllItems.Eatables.SetNum(AllItems.Eatables.Num() + AddToEatables);
	Cast<AZelda>(GetOwner())->UpdateInventory(AllItems);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	FItemStruct ItemStruct;
	AllItems.Sword.Init(ItemStruct, 5);
	AllItems.Shield.Init(ItemStruct, 4);
	AllItems.Eatables.Init(ItemStruct, 10);
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	check(PCM);

	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	const FVector Start = PCM->GetCameraLocation();
	const FVector End = Start + (PCM->GetActorForwardVector() * 700.f);

	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowTraceDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true
	);



	PrevItem = ThisItem;
	ThisItem = Cast<AItem>(HitResult.GetActor());

	if (PrevItem == nullptr)
	{
		if(ThisItem) ThisItem->ShowInteractWidget(true);
	}
	else
	{
		if (ThisItem)
		{
			if (PrevItem != ThisItem)
			{
				ThisItem->ShowInteractWidget(true);
				PrevItem->ShowInteractWidget(false);
			}
		}
		else if(ThisItem == nullptr)
		{
			PrevItem->ShowInteractWidget(false);
		}
	}
}

