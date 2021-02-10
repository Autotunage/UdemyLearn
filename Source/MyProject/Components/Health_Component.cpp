// Fill out your copyright notice in the Description page of Project Settings.


#include "Health_Component.h"
#include "GameFramework/Actor.h"
#include "MyProject/MyProjectCharacter.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)


UHealth_Component::UHealth_Component()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UHealth_Component::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;

	AActor* ComponentOwner = GetOwner();
	
	ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealth_Component::OnTakeAnyDamage);
	
	
}

void UHealth_Component::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
    class AController* InstigatedBy, AActor* DamageCauser)
{
	Health -= Damage;
	UE_LOG(LogHealthComponent, Error, TEXT("Damage given: %f"), Damage);
}
