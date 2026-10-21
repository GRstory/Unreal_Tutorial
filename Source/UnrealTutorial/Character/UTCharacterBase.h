// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UTCharacterBase.generated.h"

UENUM()
enum class ECharacterControlType : uint8
{
	TPS,
	FPS
};

UCLASS()
class UNREALTUTORIAL_API AUTCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AUTCharacterBase();

	
protected: //Components
	virtual void SetCharacterControlData(const class UUTCharacterControlData* ControlData);

	UPROPERTY()
	TMap<ECharacterControlType, class UUTCharacterControlData*> ControlDataMap;
};
