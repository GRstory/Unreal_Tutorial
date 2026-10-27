// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/UTCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/Data/UTCharacterControlData.h"

AUTCharacterBase::AUTCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

    //콜라이더
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()-> SetCollisionProfileName(TEXT("Pawn"));

    //Pawn
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    //Movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

    //Skeletal Mesh
    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
    GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
    GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Sample/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"));
    if (CharacterMeshRef.Object)
    {
        GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
    }

    static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Sample/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny_C"));
    if (AnimInstanceClassRef.Class)
    {
        GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
    }

    static ConstructorHelpers::FObjectFinder<UUTCharacterControlData> TPSDataRef(TEXT("/Script/UnrealTutorial.UTCharacterControlData'/Game/Character/Control/DA_FPS.DA_FPS'"));
    if (TPSDataRef.Object)
    {
        ControlDataMap.Add(ECharacterControlType::TPS, TPSDataRef.Object);
    }
    static ConstructorHelpers::FObjectFinder<UUTCharacterControlData> FPSDataRef(TEXT("/Script/UnrealTutorial.UTCharacterControlData'/Game/Character/Control/DA_TPS.DA_TPS'"));

    if (FPSDataRef.Object)
    {
        ControlDataMap.Add(ECharacterControlType::FPS, FPSDataRef.Object);
    }
}

void AUTCharacterBase::SetCharacterControlData(const UUTCharacterControlData* ControlData)
{
    bUseControllerRotationYaw = ControlData-> bUseControllerRotationYaw;

    UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
    CharacterMovementComponent->bOrientRotationToMovement = ControlData->bOrientRotationToMovement;
    CharacterMovementComponent->bUseControllerDesiredRotation = ControlData->bUseControllerDesiredRotation;
    CharacterMovementComponent->RotationRate = ControlData->RotationRate;

}

bool AUTCharacterBase::PlayActionMontage(UAnimMontage* Montage, FOnMontageEnded& Delegate)
{
    if (Montage == nullptr)
    {
        return false;
    }

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    if (AnimInstance)
    {
        if (AnimInstance->Montage_Play(Montage))
        {
            AnimInstance->Montage_SetEndDelegate(Delegate, Montage);

            return true;
        }
    }

    return false;
}

void AUTCharacterBase::DoDefaultAttack()
{
    if (bIsAttacking || !DefaultAttackMontage)
    {
        return;
    }
    
    FOnMontageEnded MontageEndDelegate;
    MontageEndDelegate.BindUObject(this, &AUTCharacterBase::OnDefaultAttackMontageEnded);
    if (PlayActionMontage(DefaultAttackMontage, MontageEndDelegate))
    {
        bIsAttacking = true;
    }
}

void AUTCharacterBase::OnDefaultAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    bIsAttacking = false;
}
