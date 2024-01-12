#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	EIT_Sword UMETA(DisplayName = "Sword"),
	EIT_Shield UMETA(DisplayName = "Shield"),
	EIT_Eatables UMETA(DisplayName = "Eatables"),
	EIT_None UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FDataTableRowHandle ThisItemTableHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemTypes ItemTypes = EItemTypes::EIT_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Quantity = 0;
};

USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		int32 StackSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Power;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TObjectPtr<UStaticMesh> Mesh;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemEquippedDelegate, FItemStruct, Value);


UCLASS()
class INVENTORYUI_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();

	virtual void Tick(float DeltaTime) override;

	void ShowInteractWidget(bool bShowWidget);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn = "true")) // ExposeOnSpawn - 프로퍼티를 클래스 유형에 대한 Spawn Actor에 연결지정합니다.
		FItemStruct ItemStruct;

	UPROPERTY(VisibleAnywhere)
	FItemInfo ItemInfo;

	FOnItemEquippedDelegate OnItemEquippedDelegate;

protected:

	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere)
	class UWidgetComponent* InteractWidget;

};
