// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FastPickUpGame/InteractSystem/FPUGInteractInterface.h"

#include "FPUGWorldItemBase.generated.h"

UCLASS()
class FASTPICKUPGAME_API AFPUGWorldItemBase : public AActor, public IFPUGInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPUGWorldItemBase();

	//Override InteractInterface functions

	void Interact(AActor* Executor) override;

	bool CanInteract(AActor* Executor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	UStaticMeshComponent* VisualMesh;


};
