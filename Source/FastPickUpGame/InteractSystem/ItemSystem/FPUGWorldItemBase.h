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

	EInteractActionType Interact(AActor* Executor) override;

	bool CanInteract(AActor* Executor) override;

public:

	void InitAppearence(UStaticMesh* Mesh, UMaterialInstance* Material);

	void SetItemID(int32 NewId);

	UFUNCTION()
	void OnRep_ItemID();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	UStaticMeshComponent* VisualMesh;

protected:

	UPROPERTY(ReplicatedUsing = "OnRep_ItemID")
	int32 ItemID = -1;

};