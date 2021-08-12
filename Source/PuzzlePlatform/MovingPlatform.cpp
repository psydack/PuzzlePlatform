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
	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);

	Super::BeginPlay();
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTriggers > 0)
	{
		if (HasAuthority())
		{
			FVector Location = GetActorLocation();
			FVector Direction = GlobalTargetLocation - GlobalStartLocation;

			float JourneyLength = Direction.Size();
			float JourneyTravelled = (Location - GlobalStartLocation).Size();

			if (JourneyTravelled > JourneyLength)
			{
				FVector Swap = GlobalStartLocation;
				GlobalStartLocation = GlobalTargetLocation;
				GlobalTargetLocation = Swap;
			}

			FVector DirectionNormal = Direction.GetSafeNormal();
			Location += DirectionNormal * MoveSpeed * DeltaTime;
			SetActorLocation(Location);
		}
	}
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	ActiveTriggers -= ActiveTriggers > 0 ? 1 : 0;
}
