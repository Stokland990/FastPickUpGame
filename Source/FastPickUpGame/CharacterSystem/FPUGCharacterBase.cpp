// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPUGCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

#include "FastPickUpGame/InteractSystem/FPUGInteractComponent.h"
#include "FastPickUpGame/GameplaySystem/FPUGGameModeBase.h"


AFPUGCharacterBase::AFPUGCharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	InteractComponent = CreateDefaultSubobject<UFPUGInteractComponent>(TEXT("InteractComponent"));
}

void AFPUGCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLocallyControlled())
	{
		InteractComponent->CosmeticTrace();
	}
}

void AFPUGCharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPUGCharacterBase::PlayerInteract);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFPUGCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFPUGCharacterBase::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
}

void AFPUGCharacterBase::PlayerInteract()
{
	if (InteractComponent)
	{
		InteractComponent->Interact();
	}
}

void AFPUGCharacterBase::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFPUGCharacterBase::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

int32 AFPUGCharacterBase::GetItemIdToCollectInternal() const
{
	return ItemIdToCollect;
}

void AFPUGCharacterBase::OnRep_ItemIdItemIdToCollect()
{
	OnItemIdUpdated.Broadcast(ItemIdToCollect);
}

void AFPUGCharacterBase::PickUpScoreItem()
{
	const auto GM = GetWorld()->GetAuthGameMode<AFPUGGameModeBase>();

	if (GM)
	{
		const int32 TeamId = GetPlayerState()->GetPlayerId();

		GM->AddScoreToTeamById(TeamId, 1);
	}
}

//Deciding what to do with interact information
void AFPUGCharacterBase::HandleInteractAction(EInteractActionType Type)
{
	switch (Type)
	{
	case EInteractActionType::ScorePickUp:

		PickUpScoreItem();

		break;

	default:
		break;
	}
}

//Get component from which interact trace will be made.
USceneComponent* AFPUGCharacterBase::GetComponentForInteractTrace() const
{
	return FollowCamera;
}

void AFPUGCharacterBase::SetItemIdToCollect(const int32 NewId)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AFPUGCharacterBase, ItemIdToCollect, this);

	ItemIdToCollect = NewId;
}

int32 AFPUGCharacterBase::GetItemIdToCollect() const
{
	return ItemIdToCollect;
}

void AFPUGCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.Condition = COND_None;

	DOREPLIFETIME_WITH_PARAMS_FAST(AFPUGCharacterBase, ItemIdToCollect, SharedParams);
}