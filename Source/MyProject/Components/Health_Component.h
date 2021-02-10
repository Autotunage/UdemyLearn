// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health_Component.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UHealth_Component : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHealth_Component();

	float GetHealth() const {return Health;}
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float MaxHealth = 100.0f;
	
	virtual void BeginPlay() override;

private:
	float Health = 0.0f;

	UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
    class AController* InstigatedBy, AActor* DamageCauser);
	
};
