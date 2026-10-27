// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UTAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREALTUTORIAL_API UUTAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UUTAnimInstance();

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<class UCharacterMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float MovingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	uint8 bIsOnAir : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	float JumpingThreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	uint8 bIsJumping : 1;
};
