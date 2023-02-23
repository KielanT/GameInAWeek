// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "SwordActor.h"

// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
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
	
	//Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, FName("LeftHand"));
	SpawnSword();
	
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
    		//if(MoveHorizontalAction)
    		//	InputComp->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &ABasicCharacter::Move);

			if(LungeAction)
			{
				InputComp->BindAction(LungeAction, ETriggerEvent::Started, this, &ABasicCharacter::Lunge);
				InputComp->BindAction(LungeAction, ETriggerEvent::Completed, this, &ABasicCharacter::Retreat);
			}

			if(ArmAction)
			{
				InputComp->BindAction(ArmAction, ETriggerEvent::Triggered, this, &ABasicCharacter::RotateArm);
			}
    	}

}

void ABasicCharacter::Move(const FInputActionValue& Value)
{
	float MovementVector = Value.Get<float>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		AddMovementInput(Direction, MovementVector);
	}
}

void ABasicCharacter::Lunge()
{
	bIsLunge = true;
	bIsRetreat = false;
}

void ABasicCharacter::Retreat()
{
	bIsRetreat = true;
	bIsLunge = false;
}

void ABasicCharacter::RotateArm(const FInputActionValue& Value)
{
	float input = Value.Get<float>() * ArmSpeed;
	
	ArmDeltaPitch = FMath::Clamp(input + ArmDeltaPitch, ArmMinClamp, ArmMaxClamp);;
	
}

void ABasicCharacter::SpawnSword()
{
	if(SwordActorClass)
	{
		FActorSpawnParameters Params;
		FAttachmentTransformRules Rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		//Rules.LocationRule = EAttachmentRule::SnapToTarget;
		Rules.RotationRule = EAttachmentRule::KeepWorld;
		//Rules.ScaleRule = EAttachmentRule::SnapToTarget;
		
		SwordActor = GetWorld()->SpawnActor<ASwordActor>(SwordActorClass, GetTransform(), Params);
		SwordActor->AttachToComponent(GetMesh(), Rules, FName("WeaponSocket"));
	}
}

