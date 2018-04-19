// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BEGINNINGPROJECT_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
  float reach = 200.0f;

  UPhysicsHandleComponent *physicsHandle = nullptr;
  UInputComponent *input = nullptr;

  // Ray-cast and grab whats in reach
  void Grab();
  void Release();

  FVector getLineTraceStart();
  FVector getLineTraceEnd();

  // Find (assumed) attached physics handle
  void findPhysicsHandleComponent();

  // Setup (assumed) attached input component
  void SetupInputComponent();

  // Return hit for first physics body in reachs
  const FHitResult getFirstPhysicsBodyInReach();
};
