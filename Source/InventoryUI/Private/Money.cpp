#include "Money.h"
#include "MoneyDataTable.h"
#include "Engine/DataTable.h"
#include "Zelda.h"
#include "InventoryComponent.h"

AMoney::AMoney()
{
	PrimaryActorTick.bCanEverTick = true;

	MoneyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Money Mesh");
	MoneyMesh->SetupAttachment(RootComponent);
	MoneyMesh->SetGenerateOverlapEvents(true);
}

void AMoney::BeginPlay()
{
	Super::BeginPlay();

	MoneyMesh->OnComponentBeginOverlap.AddDynamic(this, &AMoney::BeginOverlap);
}

void AMoney::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	FName Name = MoneyTableHandle.RowName; //DataTableRowHandle에서 설정한 FMoneyStruct의 RowName.

	MoneyStruct = *MoneyTableHandle.DataTable->FindRow<FMoneyStruct>(Name, ""); //DataTableRowHandle에서 설정한 DataTable에서 Name과 같은 Row의 정보를 가져옴.

	MoneyMesh->SetStaticMesh(MoneyStruct.Mesh); // 가져온 정보의 Mesh로 MoneyMesh를 설정함.

}

void AMoney::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AZelda* Zelda = Cast<AZelda>(OtherActor);
	check(Zelda);

	FName Name = MoneyTableHandle.RowName;

	MoneyStruct = *MoneyTableHandle.DataTable->FindRow<FMoneyStruct>(Name, "");

	int64 Money = Zelda->InventoryComponent->GetMoney() + MoneyStruct.Amount;

	Zelda->InventoryComponent->SetMoney(Money);

	//GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), Money));

	Destroy();
}

void AMoney::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

