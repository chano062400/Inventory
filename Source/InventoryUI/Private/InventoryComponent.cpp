#include "InventoryComponent.h"
#include "Money.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Item.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


	FItemStruct ItemStruct;
	AllItems.Sword.Init(ItemStruct, 5);
	AllItems.Shield.Init(ItemStruct, 4);
	AllItems.Eatables.Init(ItemStruct, 10);
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
		int CanAddIdx = 6;
		bool CanEqualItem = false; 
		for (int idx = 0; idx < AllItems.Sword.Num(); idx++)
		{
			auto& Item = AllItems.Sword[idx];
			if(Item.ItemTypes == EItemTypes::EIT_None) CanAddIdx = FMath::Min(CanAddIdx, idx); // ���� �߰��� �� �ִ� ��ġ 

			if (CanAddIdx == 0) // �ش� �������� �κ��丮�� �ƹ��͵� ���� ���.
			{
				AllItems.Sword.Insert(ItemToAdd->ItemStruct, CanAddIdx);
				AllItems.Sword.SetNum(LimitSize); // Insert�� ����� 1���� �þ�� ������ �ִ� �κ��丮 ������� ������.
				return true;
			}
			
			if (Item.ThisItemTableHandle.RowName == ItemToAdd->ItemStruct.ThisItemTableHandle.RowName)
			{
				FName Name = Item.ThisItemTableHandle.RowName; 
				FItemInfo* ItemInfo = Item.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");
				int32 QuantitiyToAdd = Item.Quantity + ItemToAdd->ItemStruct.Quantity; // �߰��ǰ� ������ �� 

				if (ItemInfo && QuantitiyToAdd <= ItemInfo->StackSize)
				{
					int32 ItemQuantity = FMath::Clamp(QuantitiyToAdd, 0, ItemInfo->StackSize); // Clamp�� �̿��ؼ� ���� �� �ִ� �ִ뷮(stacksize) ���Ϸ� �߰�.
					Item.Quantity = ItemQuantity;
					CanEqualItem = true;
					AllItems.Sword.SetNum(LimitSize);
					return true;
				}
				else // �ִ뷮���� ������ �ִٸ� �� �̻� ���� �� ����.
				{
					CanEqualItem = true;
					//To Do (�ִ� ������ ���� �ִٴ� �޼��� ���� ���)
					return false;
				}
			}
		}

		if (!CanEqualItem && CanAddIdx > 0) // �ش� �������� �κ��丮�� �������� ������ ���� �������� ���� ���.
		{
			AllItems.Sword.Insert(ItemToAdd->ItemStruct, CanAddIdx);
			AllItems.Sword.SetNum(LimitSize); 
			return true;
		}


		return false;
	}

	if (ItemToAdd->ItemStruct.ItemTypes == EItemTypes::EIT_Shield)
	{
		int32 LimitSize = AllItems.Shield.Num();
		int CanAddIdx = 6;
		bool CanEqualItem = false;
		for (int idx = 0; idx < AllItems.Shield.Num(); idx++)
		{
			auto& Item = AllItems.Shield[idx];
			if (Item.ItemTypes == EItemTypes::EIT_None) CanAddIdx = FMath::Min(CanAddIdx, idx); // ���� �߰��� �� �ִ� ��ġ 

			if (CanAddIdx == 0) // �ش� �������� �κ��丮�� �ƹ��͵� ���� ���.
			{
				AllItems.Shield.Insert(ItemToAdd->ItemStruct, CanAddIdx);
				AllItems.Shield.SetNum(LimitSize);
				return true;
			}

			if (Item.ThisItemTableHandle.RowName == ItemToAdd->ItemStruct.ThisItemTableHandle.RowName)
			{
				FName Name = Item.ThisItemTableHandle.RowName;
				FItemInfo* ItemInfo = Item.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");
				int32 QuantitiyToAdd = Item.Quantity + ItemToAdd->ItemStruct.Quantity; // �߰��ǰ� ������ �� 

				if (ItemInfo && QuantitiyToAdd <= ItemInfo->StackSize)
				{
					int32 ItemQuantity = FMath::Clamp(QuantitiyToAdd, 0, ItemInfo->StackSize); // Clamp�� �̿��ؼ� ���� �� �ִ� �ִ뷮(stacksize) ���Ϸ� �߰�.
					Item.Quantity = ItemQuantity;
					CanEqualItem = true;
					AllItems.Shield.SetNum(LimitSize);
					return true;
				}
				else // �ִ뷮���� ������ �ִٸ� �� �̻� ���� �� ����.
				{
					CanEqualItem = true;
					//To Do (�ִ� ������ ���� �ִٴ� �޼��� ���� ���)
					return false;
				}
			}
		}
		if (!CanEqualItem && CanAddIdx > 0) // �ش� �������� �κ��丮�� �������� ������ ���� �������� ���� ���.
		{
			AllItems.Shield.Insert(ItemToAdd->ItemStruct, CanAddIdx);
			AllItems.Shield.SetNum(LimitSize);  // Insert�� ����� 1���� �þ�� ������ �ִ� �κ��丮 ������� ������.
			return true;
		}


		return false;
	}

	if (ItemToAdd->ItemStruct.ItemTypes == EItemTypes::EIT_Eatables)
	{
		int32 LimitSize = AllItems.Eatables.Num();
		int CanAddIdx = 6;
		bool CanEqualItem = false;
		for (int idx = 0; idx < AllItems.Eatables.Num(); idx++)
		{
			auto& Item = AllItems.Eatables[idx];
			if (Item.ItemTypes == EItemTypes::EIT_None) CanAddIdx = FMath::Min(CanAddIdx, idx); // ���� �߰��� �� �ִ� ��ġ 

			if (CanAddIdx == 0) // �ش� �������� �κ��丮�� �ƹ��͵� ���� ���.
			{
				AllItems.Eatables.Insert(ItemToAdd->ItemStruct, CanAddIdx);
				AllItems.Eatables.SetNum(LimitSize); // Insert�� ����� 1���� �þ�� ������ �ִ� �κ��丮 ������� ������.
				return true;
			}

			if (Item.ThisItemTableHandle.RowName == ItemToAdd->ItemStruct.ThisItemTableHandle.RowName)
			{
				FName Name = Item.ThisItemTableHandle.RowName;
				FItemInfo* ItemInfo = Item.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");
				int32 QuantitiyToAdd = Item.Quantity + ItemToAdd->ItemStruct.Quantity; // �߰��ǰ� ������ �� 

				if (ItemInfo && QuantitiyToAdd <= ItemInfo->StackSize)
				{
					int32 ItemQuantity = FMath::Clamp(QuantitiyToAdd, 0, ItemInfo->StackSize); // Clamp�� �̿��ؼ� ���� �� �ִ� �ִ뷮(stacksize) ���Ϸ� �߰�.
					Item.Quantity = ItemQuantity;
					CanEqualItem = true;
					AllItems.Eatables.SetNum(LimitSize);
					return true;
				}
				else // �ִ뷮���� ������ �ִٸ� �� �̻� ���� �� ����.
				{
					CanEqualItem = true;
					//To Do (�ִ� ������ ���� �ִٴ� �޼��� ���� ���)
					return false;
				}
			}
		}
		if (!CanEqualItem && CanAddIdx > 0) // �ش� �������� �κ��丮�� �������� ������ ���� �������� ���� ���.
		{
			AllItems.Eatables.Insert(ItemToAdd->ItemStruct, CanAddIdx);
			AllItems.Eatables.SetNum(LimitSize); 
			return true;
		}
	}
	
	return false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
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

