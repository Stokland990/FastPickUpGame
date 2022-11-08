// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPUGDoorBase.generated.h"

UCLASS()
class FASTPICKUPGAME_API AFPUGDoorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPUGDoorBase();

public:

	void Tick(float DeltaTime) override;

public:

	void ToggleDoorState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_IsOpened();

private:

	void ToggleDoorPosition();
public:

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	UStaticMeshComponent* VisualMesh;

	UPROPERTY(VisibleAnywhere, Category = "Visual")
	USceneComponent* RootPoint;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "DoorSettings")
	FVector OpenDoorPosition = FVector(0.f, 0.f, 200.f);

	UPROPERTY(EditDefaultsOnly, Category = "DoorSettings")
	FVector CloseDoorPosition = FVector(0.f);

	UPROPERTY(EditDefaultsOnly, Category = "DoorSettings")
	float DoorSpeed = 10.f;

private:
	FVector TargetPosition;
	FVector CurrentPosition;

	UPROPERTY(ReplicatedUsing = "OnRep_IsOpened")
	bool bShouldOpen = true;
};
