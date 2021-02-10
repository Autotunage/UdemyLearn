
#include "MyProjectCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BaseCharMovementComponent.h"
#include "Components/Health_Component.h"
#include "Components/TextRenderComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

AMyProjectCharacter::AMyProjectCharacter(const FObjectInitializer& ObjInit)
	:Super(ObjInit.SetDefaultSubobjectClass<UBaseCharMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	HealthComponent = CreateDefaultSubobject<UHealth_Component>(TEXT("HealthComponent"));
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("HealthTextComponent"));
	HealthTextComponent->SetupAttachment(GetRootComponent());
	

}

void AMyProjectCharacter::BeginPlay()
{
	Super::BeginPlay();

}


void AMyProjectCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const auto Healths = HealthComponent->GetHealth();
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Healths)));
	
	//TakeDamage(0.1f, FDamageEvent{}, Controller, this);
}


void AMyProjectCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyProjectCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProjectCharacter::MoveRight);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AMyProjectCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AMyProjectCharacter::OnStopRunning);

}

void AMyProjectCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		//Расчет вектора направления и итоговый поворот персонажа
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		

		//AddMovementInput(GetActorForwardVector(), Value); //Движение без поворота (чисто обработкой анимации в блюпринтах)
	}
}

void AMyProjectCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
		

		//AddMovementInput(GetActorRightVector(), Value);
	}
}


void AMyProjectCharacter::OnStartRunning()
{
	WantsToRun = true;
}

void AMyProjectCharacter::OnStopRunning()
{
	WantsToRun = false;
}

bool AMyProjectCharacter::IsRunning() const
{
	return WantsToRun && !GetVelocity().IsZero();
}


float AMyProjectCharacter::GetMovementDirection() const
{
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngelBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

	return FMath::RadiansToDegrees(AngelBetween) * FMath::Sign(CrossProduct.Z);
}