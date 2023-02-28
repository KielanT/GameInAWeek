// Fill out your copyright notice in the Description page of Project Settings.

#include "RhythmCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "SwordActor.h"

// Rhythm Character has different controls and animations
// If the player holds w and then left mouse click, then it does a high lunge 
// If the player just does a left mouse click, then it does a centre lunge 
// If the player holds s and then left mouse click, then it does a low lunge 

// Sets default values
ARhythmCharacter::ARhythmCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void ARhythmCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* SubsystemInterface = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubsystemInterface->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	SpawnSword();

	// Sets default value
	LungeState = ELungeState::Lunge;
}

// Called every frame
void ARhythmCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARhythmCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// Sets up the enhanced input
	if(UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(LungeAction)
		{
			InputComp->BindAction(LungeAction, ETriggerEvent::Started, this, &ARhythmCharacter::Lunge);
		}

		if(HighLungeAction)
		{
			InputComp->BindAction(HighLungeAction, ETriggerEvent::Started, this, &ARhythmCharacter::HighLunge);
			InputComp->BindAction(HighLungeAction, ETriggerEvent::Completed, this, &ARhythmCharacter::ResetLunge);
		}

		if(LowLungeAction)
		{
			InputComp->BindAction(LowLungeAction, ETriggerEvent::Started, this, &ARhythmCharacter::LowLunge);
			InputComp->BindAction(LowLungeAction, ETriggerEvent::Completed, this, &ARhythmCharacter::ResetLunge);
		}
	}
}

void ARhythmCharacter::Lunge()
{
	// Plays the animation depending on the type of lunge
	if(LungeState == ELungeState::Lunge)
	{
		if(LungeMontage && GetCurrentMontage() != LungeMontage)
			PlayAnimMontage(LungeMontage);
	}
	else if(LungeState == ELungeState::LowLunge)
	{
		if(LungeLowMontage && GetCurrentMontage() != LungeLowMontage)
			PlayAnimMontage(LungeLowMontage);
	}
	else if(LungeState == ELungeState::HighLunge)
	{
		if(LungeHighMontage && GetCurrentMontage() != LungeHighMontage)
			PlayAnimMontage(LungeHighMontage);
	}
}

void ARhythmCharacter::LowLunge()
{
	LungeState = ELungeState::LowLunge; // Sets low lunge
}

void ARhythmCharacter::HighLunge()
{
	LungeState = ELungeState::HighLunge; // Sets high lunge
}

void ARhythmCharacter::ResetLunge()
{
	LungeState = ELungeState::Lunge; // Resets the lunge to allow the centre lunge
}

void ARhythmCharacter::SpawnSword()
{
	// Spawns the sword, exactly the same as the other character
	if(SwordActorClass)
	{
		FActorSpawnParameters Params;
		FAttachmentTransformRules Rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		Rules.RotationRule = EAttachmentRule::KeepWorld;
		
		SwordActor = GetWorld()->SpawnActor<ASwordActor>(SwordActorClass, GetTransform(), Params);
		SwordActor->AttachToComponent(GetMesh(), Rules, FName("WeaponSocket"));
	}
}


