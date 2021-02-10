// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "MyProjectCharacter.h"
AMainGameModeBase::AMainGameModeBase()
{
	DefaultPawnClass = AMyProjectCharacter::StaticClass();
};