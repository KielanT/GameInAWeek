// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	TempStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Temp Static Mesh"));
	TempStaticMesh->SetupAttachment(GetCapsuleComponent());

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->SetupAttachment(GetCapsuleComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* SubsystemInterface = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubsystemInterface->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    	{
    		//Binds
    		if(MoveHorizontalAction)
    			InputComp->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &ABasicCharacter::Move);

			if(LungeAction)
				InputComp->BindAction(LungeAction, ETriggerEvent::Started, this, &ABasicCharacter::Lunge);
    	}

}

void ABasicCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABasicCharacter::Lunge()
{
	UE_LOG(LogTemp, Warning, TEXT("Lunge"));
}

