// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "FPUGPickUpInterface.h"
#include "FastPickUpGame/InteractSystem/FPUGInteractExecutorInterface.h"

#include "FPUGCharacterBase.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFPUGInteractComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemIdUpdated, const int32, NewId);

UCLASS(config = Game)
class AFPUGCharacterBase : public ACharacter, public IFPUGPickUpInterface, public IFPUGInteractExecutorInterface
{
	GENERATED_BODY()


public:
	AFPUGCharacterBase();

	void Tick(float DeltaTime) override;

	// --- Override Interface functions

	void HandleInteractAction(EInteractActionType Type) override;

	USceneComponent* GetComponentForInteractTrace() const override;

	void SetItemIdToCollect(const int32 NewId) override;

	int32 GetItemIdToCollect() const override;

	// --- New functions

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	int32 GetItemIdToCollectInternal() const;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:

	void MoveForward(float Value);

	void MoveRight(float Value);

	//APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void PlayerInteract();

	void PickUpScoreItem();

	UFUNCTION()
	void OnRep_ItemIdItemIdToCollect();
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnItemIdUpdated OnItemIdUpdated;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	//Component that handles interact actions
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	UFPUGInteractComponent* InteractComponent;

private:

	UPROPERTY(ReplicatedUsing = "OnRep_ItemIdItemIdToCollect")
	int32 ItemIdToCollect = -1;
};

