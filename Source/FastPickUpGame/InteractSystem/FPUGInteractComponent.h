// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPUGInteractComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FASTPICKUPGAME_API UFPUGInteractComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFPUGInteractComponent();

	// --- New functions

	void Interact();

	void CosmeticTrace();

private:

	void InteractInternal();

	static void ToggleInteractComponentVisuals(UPrimitiveComponent* InteractComponent, const bool bShouldSwitchOn);

	USceneComponent* GetExecutorTraceComponent();

	FHitResult TraceForInteract();

	UFUNCTION(Server, Reliable)
	void ServerInteract();

protected: 

	UPROPERTY(EditDefaultsOnly, Category = "InteractInfo")
	float InteractDistance = 500.f;

private:

	UPROPERTY()
	USceneComponent* ExecutorTraceComponent;

	UPROPERTY()
	UPrimitiveComponent* ComponentToInteractWith;
		
};
