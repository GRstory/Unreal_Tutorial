// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/UTAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UTAnimInstance.h"

UUTAnimInstance::UUTAnimInstance()
{
	MovingThreshold = 3.0f;
	JumpingThreshold = 100.0f;
}

void UUTAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		MovementComponent = Cast<UCharacterMovementComponent>(Owner->GetMovementComponent());
	}
}

void UUTAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (MovementComponent)
	{
		Velocity = MovementComponent->Velocity;
		GroundSpeed = Velocity.Size2D();

		bIsIdle = GroundSpeed < MovingThreshold;
		bIsOnAir = MovementComponent->IsFalling();
		bIsJumping = bIsOnAir & (Velocity.Z > JumpingThreshold);
	}
}
