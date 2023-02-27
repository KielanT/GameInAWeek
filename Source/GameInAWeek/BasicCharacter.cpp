// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "SwordActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameInAWeekGameMode.h"


// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast<AGameInAWeekGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		
		if(UEnhancedInputLocalPlayerSubsystem* SubsystemInterface = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubsystemInterface->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	SpawnSword();
	
	StartingRotation = GetActorRotation();

	IsPaused = false;
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bIsActive = GameModeRef->IsPlaying();
	
}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    	{
			if(LungeAction)
			{
				InputComp->BindAction(LungeAction, ETriggerEvent::Triggered, this, &ABasicCharacter::Lunge);
			}

			if(ArmAction)
			{
				InputComp->BindAction(ArmAction, ETriggerEvent::Triggered, this, &ABasicCharacter::RotateArm);
			}

			if(PauseAction)
			{
				InputComp->BindAction(PauseAction, ETriggerEvent::Started, this, &ABasicCharacter::Pause);
			}
    	}

}


void ABasicCharacter::Lunge()
{
	if(bIsActive)
	{
		if(LungeMontage && GetCurrentMontage() != LungeMontage) // Detecting the montage playing stops the player spamming
		{
			PlayAnimMontage(LungeMontage);
		}
	}
}


void ABasicCharacter::RotateArm(const FInputActionValue& Value)
{
	if(bIsActive)
	{
		float input = Value.Get<float>() * ArmSpeed;
	
		ArmDeltaPitch = FMath::Clamp(input + ArmDeltaPitch, ArmMinClamp, ArmMaxClamp);;
	}
}

void ABasicCharacter::Pause()
{
	UE_LOG(LogTemp, Warning, TEXT("Pause"));
	IsPaused = !IsPaused;
	if(IsPaused)
	{
		GameModeRef->Pause();
	}
	else if (!IsPaused)
	{
		GameModeRef->Resume();
	}
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

