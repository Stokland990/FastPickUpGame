// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGWorldItemBase.h"

#include "Net/UnrealNetwork.h"

#include "FPUGItemData.h"

// Sets default values
AFPUGWorldItemBase::AFPUGWorldItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(VisualMesh);
	VisualMesh->bRenderCustomDepth = false;
	VisualMesh->CustomDepthStencilValue = 0;

	bReplicates = true;
	bNetLoadOnClient = true;

}

// Called when the game starts or when spawned
void AFPUGWorldItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPUGWorldItemBase::OnRep_ItemID()	
{
	const FString Context;

	const FName RowName = *FString::FromInt(ItemID);

	const auto World = GetWorld();

	if (!World)
	{
		return;
	}

	const auto ItemsDT = LoadObject<UDataTable>(GetTransientPackage(), TEXT("/Game/Core/InteractSystem/ItemSystem/DT_Items"));


	if (ItemsDT)
	{
		const auto ItemInfo = ItemsDT->FindRow<FItemInfoBase>(RowName, Context);

		InitAppearance(ItemInfo->Mesh.LoadSynchronous(), ItemInfo->MaterialOverride.LoadSynchronous());
	}
}

EInteractActionType AFPUGWorldItemBase::Interact(AActor* Executor)
{
	Destroy();

	return EInteractActionType::None;
}

bool AFPUGWorldItemBase::CanInteract(AActor* Executor)
{
	return false;
}

void AFPUGWorldItemBase::InitAppearance(UStaticMesh* Mesh, UMaterialInstance* Material) const
{
	VisualMesh->SetStaticMesh(Mesh);

	VisualMesh->SetMaterial(0, Material);
}

void AFPUGWorldItemBase::SetItemID(int32 NewId)
{
	ItemID = NewId;
}

void AFPUGWorldItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPUGWorldItemBase, ItemID);

}

