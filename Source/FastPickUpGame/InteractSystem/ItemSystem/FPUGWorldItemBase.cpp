// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGWorldItemBase.h"

// Sets default values
AFPUGWorldItemBase::AFPUGWorldItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(VisualMesh);

	bReplicates = true;

}

// Called when the game starts or when spawned
void AFPUGWorldItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPUGWorldItemBase::Interact(AActor* Executor)
{
	Destroy();
}

bool AFPUGWorldItemBase::CanInteract(AActor* Executor)
{
	return false;
}
