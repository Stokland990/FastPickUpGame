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
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);
}

void UFPUGInteractComponent::Interact()
{
	//If Host or single player - do interact, otherwise send RPC
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		InteractInternal();
	}
	else
	{
		ServerInteract();
	}
}

void UFPUGInteractComponent::CosmeticTrace()
{
	const auto TraceResult = TraceForInteract();

	if (TraceResult.bBlockingHit)
	{
		IFPUGInteractInterface* InteractInterface = Cast<IFPUGInteractInterface>(TraceResult.GetActor());

		if (InteractInterface)
		{
			const bool CanInteract = InteractInterface->CanInteract(GetOwner());

			if (CanInteract)
			{
				if (ComponentToInteractWith != TraceResult.GetComponent())
				{
					if (ComponentToInteractWith)
					{
						ToggleInteractComponentVisuals(ComponentToInteractWith, false);
					}

					ComponentToInteractWith = TraceResult.GetComponent();

					ToggleInteractComponentVisuals(ComponentToInteractWith, true);
				}
				
				return;
			}
		}
	}

	if (ComponentToInteractWith)
	{
		ToggleInteractComponentVisuals(ComponentToInteractWith, false);

		ComponentToInteractWith = nullptr;
	}
}

void UFPUGInteractComponent::InteractInternal()
{
	const auto TraceResult = TraceForInteract();

	if (TraceResult.bBlockingHit)
	{
		IFPUGInteractInterface* InteractInterface = Cast<IFPUGInteractInterface>(TraceResult.GetActor());

		if (InteractInterface)
		{
			const bool CanInteract = InteractInterface->CanInteract(GetOwner());

			if (CanInteract)
			{
				const auto InteractType = InteractInterface->Interact(GetOwner());

				const auto ExecutorInterface = Cast<IFPUGInteractExecutorInterface>(GetOwner());

				if (ExecutorInterface)
				{
					ExecutorInterface->HandleInteractAction(InteractType);
				}
			}
		}
	}
}

void UFPUGInteractComponent::ToggleInteractComponentVisuals(UPrimitiveComponent* InteractComponent, const bool bShouldSwitchOn)
{
	InteractComponent->SetRenderCustomDepth(bShouldSwitchOn);

	InteractComponent->SetCustomDepthStencilValue(bShouldSwitchOn ? 2 : 0);
}

USceneComponent* UFPUGInteractComponent::GetExecutorTraceComponent()
{
	if (!ExecutorTraceComponent)
	{
		const auto ExecutorInterface = Cast<IFPUGInteractExecutorInterface>(GetOwner());

		if (ExecutorInterface)
		{
			ExecutorTraceComponent = ExecutorInterface->GetComponentForInteractTrace();
		}
	}

	return ExecutorTraceComponent;
}

//Making trace for interact
FHitResult UFPUGInteractComponent::TraceForInteract()
{
	FHitResult OutHit;

	//Getting trace component from Executor
	const auto TraceComp = GetExecutorTraceComponent();
	
	if (TraceComp)
	{
		const FVector TraceStart = TraceComp->GetComponentLocation();
		const FVector TraceEnd = TraceComp->GetForwardVector() * InteractDistance + TraceStart;

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