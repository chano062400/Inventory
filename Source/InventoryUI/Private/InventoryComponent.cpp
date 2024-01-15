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
		int CanAddIdx = LimitSize - 1; // ���� �߰��� �� �ִ� ��ġ 
		bool IsInInventoryThisItem = false; 
		for (int idx = 0; idx < AllItems.Sword.Num(); idx++)
		{
			auto& Item = AllItems.Sword[idx];
			if(Item.ItemTypes == EItemTypes::EIT_None) CanAddIdx = FMath::Min(CanAddIdx, idx); 

			if (CanAddIdx == 0) // �ش� ������ �κ��丮�� �ƹ��͵� ���� ���.
			{
				//AllItems.Sword.Insert(ItemToAdd->ItemStruct, CanAddIdx);
				//AllItems.Sword.SetNum(LimitSize); // Insert�� ����� 1���� �þ�� ������ �ִ� �κ��丮 ������� ������.
				AllItems.Sword[CanAddIdx] = ItemToAdd->ItemStruct;
				return true;
			}
			
			if (Item.ThisItemTableHandle.RowName == ItemToAdd->ItemStruct.ThisItemTableHandle.RowName)// �ش� �������� �κ��丮�� �̹� �ִ� ���.
			{
				FName Name = Item.ThisItemTableHandle.RowName; 
				FItemInfo* ItemInfo = Item.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");
				int32 QuantitiyToAdd = Item.Quantity + ItemToAdd->ItemStruct.Quantity; // �߰��ǰ����� ����

				if (ItemInfo && Item.Quantity < ItemInfo->StackSize) // �ִ� �������� ���� �����ִٸ�
				{
					int32 ItemQuantity = FMath::Clamp(QuantitiyToAdd, 0, ItemInfo->StackSize); // Clamp�� �̿��ؼ� ���� �� �ִ� �ִ뷮(stacksize) ���Ϸ� �߰�.
					Item.Quantity = ItemQuantity;
					IsInInventoryThisItem = true;
					//AllItems.Sword.SetNum(LimitSize);
					return true;
				}
				else // �ִ뷮���� ������ �ִٸ� �� �̻� ���� �� ����.
				{
					IsInInventoryThisItem = true;
					//To Do (�ִ� ������ ���� �ִٴ� �޼��� ���� ���)
					return false;
				}
			}
		}

		if (!IsInInventoryThisItem && CanAddIdx > 0) // �ش� ������ �κ��丮�� �ٸ� �������� ������ �ش� �������� ���� ���.
		{
			//AllItems.Sword.Insert(ItemToAdd->ItemStruct, CanAddIdx);
			//AllItems.Sword.SetNum(LimitSize); // Insert�� ����� 1���� �þ�� ������ �ִ� �κ��丮 ������� ������.
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
			if (Item.ItemTypes == EItemTypes::EIT_None) CanAddIdx = FMath::Min(CanAddIdx, idx); // ���� �߰��� �� �ִ� ��ġ 

			if (CanAddIdx == 0) // �ش� ������ �κ��丮�� �ƹ��͵� ���� ���.
			{
				/*AllItems.Shield.Insert(ItemToAdd->ItemStruct, CanAddIdx);
				AllItems.Shield.SetNum(LimitSize);*/
				AllItems.Shield[CanAddIdx] = ItemToAdd->ItemStruct;
				return true;
			}

			if (Item.ThisItemTableHandle.RowName == ItemToAdd->ItemStruct.ThisItemTableHandle.RowName)// �ش� �������� �κ��丮�� �̹� �ִ� ���.
			{
				FName Name = Item.ThisItemTableHandle.RowName;
				FItemInfo* ItemInfo = Item.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");
				int32 QuantitiyToAdd = Item.Quantity + ItemToAdd->ItemStruct.Quantity; // �߰��ǰ� ������ �� 

				if (ItemInfo && Item.Quantity < ItemInfo->StackSize) // �ִ� �������� ���� �����ִٸ�
				{
					int32 ItemQuantity = FMath::Clamp(QuantitiyToAdd, 0, ItemInfo->StackSize); // Clamp�� �̿��ؼ� ���� �� �ִ� �ִ뷮(stacksize) ���Ϸ� �߰�.
					Item.Quantity = ItemQuantity;
					IsInInventoryThisItem = true;
					//AllItems.Shield.SetNum(LimitSize);
					return true;
				}
				else // �ִ뷮���� ������ �ִٸ� �� �̻� ���� �� ����.
				{
					IsInInventoryThisItem = true;
					//To Do (�ִ� ������ ���� �ִٴ� �޼��� ���� ���)
					return false;
				}
			}
		}
		if (!IsInInventoryThisItem && CanAddIdx > 0) // �ش� ������ �κ��丮�� �ٸ� �������� ������ �ش� �������� ���� ���.
		{
			//AllItems.Shield.Insert(ItemToAdd->ItemStruct, CanAddIdx);
			//AllItems.Shield.SetNum(LimitSize);  // Insert�� ����� 1���� �þ�� ������ �ִ� �κ��丮 ������� ������.
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
			if (Item.ItemTypes == EItemTypes::EIT_None) CanAddIdx = FMath::Min(CanAddIdx, idx); // ���� �߰��� �� �ִ� ��ġ 

			if (CanAddIdx == 0) // �ش� ������ �κ��丮�� �ƹ��͵� ���� ���.
			{
				//AllItems.Eatables.Insert(ItemToAdd->ItemStruct, CanAddIdx);
				//AllItems.Eatables.SetNum(LimitSize); // Insert�� ����� 1���� �þ�� ������ �ִ� �κ��丮 ������� ������.
				AllItems.Eatables[CanAddIdx] = ItemToAdd->ItemStruct;
				return true;
			}

			if (Item.ThisItemTableHandle.RowName == ItemToAdd->ItemStruct.ThisItemTableHandle.RowName) // �ش� �������� �κ��丮�� �̹� �ִ� ���.
			{
				FName Name = Item.ThisItemTableHandle.RowName;
				FItemInfo* ItemInfo = Item.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");
				int32 QuantitiyToAdd = Item.Quantity + ItemToAdd->ItemStruct.Quantity; // �߰��ǰ� ������ �� 

				if (ItemInfo && Item.Quantity < ItemInfo->StackSize) // �ִ� �������� ���� �����ִٸ�
				{
					int32 ItemQuantity = FMath::Clamp(QuantitiyToAdd, 0, ItemInfo->StackSize); // Clamp�� �̿��ؼ� ���� �� �ִ� �ִ뷮(stacksize) ���Ϸ� �߰�.
					Item.Quantity = ItemQuantity;
					IsInInventoryThisItem = true;
					/*AllItems.Eatables.SetNum(LimitSize);*/
					return true;
				}
				else // �ִ뷮���� ������ �ִٸ� �� �̻� ���� �� ����.
				{
					IsInInventoryThisItem = true;
					//To Do (�ִ� ������ ���� �ִٴ� �޼��� ���� ���)
					return false;
				}
			}
		}
		if (!IsInInventoryThisItem && CanAddIdx > 0) // �ش� ������ �κ��丮�� �ٸ� �������� ������ �ش� �������� ���� ���.
		{
			//AllItems.Eatables.Insert(ItemToAdd->ItemStruct, CanAddIdx);
			//AllItems.Eatables.SetNum(LimitSize); // Insert�� ����� 1���� �þ�� ������ �ִ� �κ��丮 ������� ������.
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

