// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	if (HasAuthority())
	{
		bReplicates = true;
	}

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector Location = GetActorLocation();

		FVector GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
		FVector Direction = (GlobalTargetLocation - Location).GetSafeNormal();

		Location += Direction * MoveSpeed * DeltaTime;

		SetActorLocation(Location);
	}
}