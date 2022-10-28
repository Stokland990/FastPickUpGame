// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "FPUGItemData.generated.h"


USTRUCT(BlueprintType)
struct FItemInfoBase : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Info")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Info")
    TSoftObjectPtr<UStaticMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World Info")
    TSoftObjectPtr<UMaterialInstance> MaterialOverride;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base Info")
    bool bIsEpic = false;;
};