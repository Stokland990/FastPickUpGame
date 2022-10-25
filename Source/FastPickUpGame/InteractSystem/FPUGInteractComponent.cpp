// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGInteractComponent.h"
#include "Engine/World.h"

#include "FastPickUpGame/InteractSystem/FPUGInteractInterface.h"
#include "FastPickUpGame/InteractSystem/FPUGInteractExecutorInterface.h"

// Sets default values for this component's properties
UFPUGInteractComponent::UFPUGInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UFPUGInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFPUGInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFPUGInteractComponent::Interact()
{
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		InteractInternal();
	}
	else
	{
		ServerInteract();
	}
}

void UFPUGInteractComponent::InteractInternal()
{
	auto TraceResult = TraceForInteract();

	if (TraceResult.bBlockingHit)
	{
		IFPUGInteractInterface* InteractInterface = Cast<IFPUGInteractInterface>(TraceResult.GetActor());

		if (InteractInterface)
		{
			InteractInterface->Interact(GetOwner());
		}
	}
}

USceneComponent* UFPUGInteractComponent::GetExecutorTraceComponent()
{
	if (!ExecutorTraceComponent)
	{
		auto ExecutorInterface = Cast<IFPUGInteractExecutorInterface>(GetOwner());

		if (ExecutorInterface)
		{
			ExecutorTraceComponent = ExecutorInterface->GetComponentForInteractTrace();
		}
	}

	return ExecutorTraceComponent;
}

FHitResult UFPUGInteractComponent::TraceForInteract()
{
	FHitResult OutHit;

	const auto TraceComp = GetExecutorTraceComponent();
	
	if (TraceComp)
	{
		FVector TraceStart = TraceComp->GetComponentLocation();
		FVector TraceEnd = TraceComp->GetForwardVector() * 500.f + TraceStart;

		FCollisionObjectQueryParams Params;
		Params.AddObjectTypesToQuery(ECC_WorldDynamic);
		Params.AddObjectTypesToQuery(ECC_WorldStatic);
		Params.AddObjectTypesToQuery(ECC_Pawn);

		GetWorld()->LineTraceSingleByObjectType(OutHit, TraceStart, TraceEnd, Params);
	}

	return OutHit;
}


void UFPUGInteractComponent::ServerInteract_Implementation()
{
	InteractInternal();
}