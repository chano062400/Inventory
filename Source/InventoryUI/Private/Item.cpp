#include "Item.h"
#include "Engine/DataTable.h"
#include "Components/WidgetComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	ItemMesh->SetupAttachment(RootComponent);

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>("InteractWidget");
	InteractWidget->SetupAttachment(ItemMesh);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	InteractWidget->SetVisibility(false);
}

void AItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FName Name = ItemStruct.ThisItemTableHandle.RowName;

	ItemInfo = *ItemStruct.ThisItemTableHandle.DataTable->FindRow<FItemInfo>(Name, "");

	ItemMesh->SetStaticMesh(ItemInfo.Mesh);

}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::ShowInteractWidget(bool bShowWidget)
{
	if(InteractWidget) InteractWidget->SetVisibility(bShowWidget);
}



