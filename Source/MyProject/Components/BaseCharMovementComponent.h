// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBaseCharMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "1.5", ClampMax = "10"))
	float RunModifier = 2.0f;
	
	virtual float GetMaxSpeed()const override;

	
};
