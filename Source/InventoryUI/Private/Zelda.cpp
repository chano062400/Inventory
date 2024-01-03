#include "Zelda.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "ZeldaPlayerController.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

AZelda::AZelda()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetGenerateOverlapEvents(true);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetMesh());
	SpringArm->TargetArmLength = 150.f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");

	bUseControllerRotationYaw = true;
}

void AZelda::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CharacterMappingContext, 0);
		}
	}

	UpdateInventory(InventoryComponent->AllItems);
}

void AZelda::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZelda::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	if (Controller)
	{
		const FVector FowardDirecton = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(FowardDirecton, MovementVector.X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.Y);
	}

}

void AZelda::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxis = Value.Get<FVector2D>();
	if (Controller)
	{
		AddControllerYawInput(LookAxis.X);
		AddControllerPitchInput(LookAxis.Y);
	}
}

void AZelda::Interact()
{
	if (InventoryComponent->ThisItem)
	{
		if (InventoryComponent->AddToInventory(InventoryComponent->ThisItem))
		{
			InventoryComponent->ThisItem->Destroy();
			UpdateInventory(InventoryComponent->AllItems);
		}
	}
}

void AZelda::OpenInventory()
{
	AZeldaPlayerController* PC = Cast<AZeldaPlayerController>(Controller);
	check(PC);

	PC->OpenInventory();
}

void AZelda::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZelda::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZelda::Look);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Triggered, this, &AZelda::OpenInventory);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AZelda::Interact);
	}
}
