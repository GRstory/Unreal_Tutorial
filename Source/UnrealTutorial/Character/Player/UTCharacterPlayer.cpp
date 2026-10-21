// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/UTCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Data/UTCharacterControlData.h"

AUTCharacterPlayer::AUTCharacterPlayer()
{
	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(RootComponent);
	CameraArm->TargetArmLength = 400.0f;
	CameraArm->bUsePawnControlRotation = true;

	TPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TPSCamera"));
	TPSCamera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	TPSCamera->bUsePawnControlRotation = false;


	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultIMC = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_TPSMove.IA_TPSMove'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		TPSMoveAction = InputActionMoveRef.Object;
	}


	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_TPSLook.IA_TPSLook'"));
	if (nullptr != InputActionLookRef.Object)
	{
		TPSLookAction = InputActionLookRef.Object;
	}
}

void AUTCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultIMC, 0);
		}
	}
}

void AUTCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(TPSMoveAction, ETriggerEvent::Triggered, this, &AUTCharacterPlayer::TPSMove);
	EnhancedInputComponent->BindAction(TPSLookAction, ETriggerEvent::Triggered, this, &AUTCharacterPlayer::TPSLook);
}

void AUTCharacterPlayer::SetCharacterControlData(const UUTCharacterControlData* ControlData)
{
	Super::SetCharacterControlData(ControlData);

	CameraArm->TargetArmLength = ControlData->TargetArmLength;
	CameraArm->SetRelativeRotation(ControlData->RelativeRotation);
	CameraArm->bUsePawnControlRotation = ControlData->bUsePawnControlRotation;
	CameraArm->bInheritPitch = ControlData->bInheritPitch;
	CameraArm->bInheritYaw = ControlData->bInheritYaw;
	CameraArm->bInheritRoll = ControlData->bInheritRoll;
	CameraArm->bDoCollisionTest = ControlData->bDoCollisionTest;
}

void AUTCharacterPlayer::TPSMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void AUTCharacterPlayer::TPSLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}
