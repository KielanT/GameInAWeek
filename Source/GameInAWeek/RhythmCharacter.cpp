// Fill out your copyright notice in the Description page of Project Settings.

#include "RhythmCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "SwordActor.h"

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
	LungeState = ELungeState::LowLunge;
}

void ARhythmCharacter::HighLunge()
{
	LungeState = ELungeState::HighLunge;
}

void ARhythmCharacter::ResetLunge()
{
	LungeState = ELungeState::Lunge;
}

void ARhythmCharacter::SpawnSword()
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


