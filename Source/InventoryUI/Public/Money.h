#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Money.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoneyChanged);

USTRUCT(BlueprintType)
struct FMoneyStruct : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Amount;
};

UCLASS()
class INVENTORYUI_API AMoney : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMoney();

	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	virtual void  BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MoneyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle MoneyTableHandle;

	FMoneyStruct MoneyStruct;
};
