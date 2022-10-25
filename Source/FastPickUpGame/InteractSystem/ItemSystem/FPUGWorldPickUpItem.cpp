// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGWorldPickUpItem.h"

#include "FastPickUpGame/CharacterSystem/FPUGPickUpInterface.h"

// Sets default values
AFPUGWorldPickUpItem::AFPUGWorldPickUpItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(VisualMesh);

	bReplicates = true;

}

// Called when the game starts or when spawned
void AFPUGWorldPickUpItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPUGWorldPickUpItem::Interact(AActor* Executor)
{
	IFPUGPickUpInterface* PickUpIterface = Cast<IFPUGPickUpInterface>(Executor);

	if (PickUpIterface)
	{
		PickUpIterface->PickUp(this);
	}
}

bool AFPUGWorldPickUpItem::CanInteract(AActor* Executor)
{
	return false;
}
