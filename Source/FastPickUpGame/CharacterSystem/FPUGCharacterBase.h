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

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interact", meta = (AllowPrivateAccess = "true"))
	UFPUGInteractComponent* InteractComponent;

public:
	AFPUGCharacterBase();

	void Tick(float DeltaTime) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UFUNCTION(BlueprintPure, Category = "Gameplay")
	int32 GetItemIdToCollectInternal() const;

	//Override Interface functions

	void HandleInteractAction(EInteractActionType Type) override;

	USceneComponent* GetComponentForInteractTrace() const override;

	void SetItemIdToCollect(const int32 NewId) override;

	int32 GetItemIdToCollect() const override;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

private:

	void PlayerInteract();

	void PickUpScoreItem();

	UFUNCTION()
	void OnRep_ItemIdItemIdToCollect();
	
public:

	UPROPERTY(BlueprintAssignable)
	FOnItemIdUpdated OnItemIdUpdated;

private:

	UPROPERTY(ReplicatedUsing = "OnRep_ItemIdItemIdToCollect")
	int32 ItemIdToCollect = -1;
};

