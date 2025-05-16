// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGDoorBase.h"

#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// Sets default values
AFPUGDoorBase::AFPUGDoorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RootPoint"));
	SetRootComponent(RootPoint);

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	VisualMesh->SetupAttachment(RootComponent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AFPUGDoorBase::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);
}

void AFPUGDoorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentPosition = VisualMesh->GetRelativeLocation();

	VisualMesh->SetRelativeLocation(FMath::VInterpTo(CurrentPosition, TargetPosition, DeltaTime, DoorSpeed));

	if(FMath::IsNearlyEqual(CurrentPosition.Z, TargetPosition.Z, 0.5f))
	{
		SetActorTickEnabled(false);
	}
}

void AFPUGDoorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	{
		VisualMesh->SetRelativeScale3D(DoorMeshSize);
	}
#endif
}

void AFPUGDoorBase::ToggleDoorState()
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AFPUGDoorBase, bShouldOpen, this);

	bShouldOpen = !bShouldOpen;

	ToggleDoorPosition();
}

void AFPUGDoorBase::OnRep_IsOpened()
{
	ToggleDoorPosition();
}

void AFPUGDoorBase::ToggleDoorPosition()
{
	CurrentPosition = VisualMesh->GetRelativeLocation();

	TargetPosition = bShouldOpen ? CloseDoorPosition : OpenDoorPosition;

	SetActorTickEnabled(true);
}

void AFPUGDoorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{


	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.Condition = COND_None;

	DOREPLIFETIME_WITH_PARAMS_FAST(AFPUGDoorBase, bShouldOpen, SharedParams);
}

