// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharMovementComponent.h"
#include "Myproject/MyProjectCharacter.h"


float UBaseCharMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const AMyProjectCharacter* Player = Cast<AMyProjectCharacter>(GetPawnOwner());
	
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}
