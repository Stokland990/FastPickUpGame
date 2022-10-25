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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Interact();

private:

	void InteractInternal();

	UFUNCTION(Server, Reliable)
	void ServerInteract();

	USceneComponent* GetExecutorTraceComponent();

	FHitResult TraceForInteract();

private:

	USceneComponent* ExecutorTraceComponent;
		
};
