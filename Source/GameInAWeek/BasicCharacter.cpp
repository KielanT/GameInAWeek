// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FallingActor.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "SwordActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameInAWeekGameMode.h"
#include "Components/AudioComponent.h"


// Sets default values
ABasicCharacter::ABasicCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetupAttachment(Root);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(CameraComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->SetupAttachment(GetCapsuleComponent());

	
	
}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent->Stop();
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
	IsLunging = false;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABasicCharacter::OnHit);

	PreviousHitActor = nullptr;

	
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bIsActive = GameModeRef->IsPlaying();
	SwordActor->bIsPlayerLunging = IsLunging;
	
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

			if(DodgeLeftAction)
			{
				InputComp->BindAction(DodgeLeftAction, ETriggerEvent::Completed, this, &ABasicCharacter::DodgeLeft);
			}

			if(DodgeRightAction)
			{
				InputComp->BindAction(DodgeRightAction, ETriggerEvent::Completed, this, &ABasicCharacter::DodgeRight);
			}
    	}

}


void ABasicCharacter::Lunge()
{
	if(bIsActive)
	{
		if(LungeMontage && GetCurrentMontage() != LungeMontage) // Detecting the montage playing stops the player spamming
		{
			IsLunging = true;

			if(UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
			{
				AnimInstance->Montage_Play(LungeMontage);
				
				// Must be binded after play, otherwise delegate never gets called
				FOnMontageEnded MontageEndedDelegate;
				MontageEndedDelegate.BindUObject(this, &ABasicCharacter::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, LungeMontage);
			}
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
		Rules.RotationRule = EAttachmentRule::KeepWorld;
		
		SwordActor = GetWorld()->SpawnActor<ASwordActor>(SwordActorClass, GetTransform(), Params);
		SwordActor->AttachToComponent(GetMesh(), Rules, FName("WeaponSocket"));
	}
}

void ABasicCharacter::DodgeRight()
{
	StartPos = GetActorLocation();
	FVector loc = StartPos;
	loc.Y = -DodgeValue;
	SetActorLocation(loc);
	GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &ABasicCharacter::ResetDodge, DodgeTimer, false);
}

void ABasicCharacter::DodgeLeft()
{
	StartPos = GetActorLocation();
	FVector loc = StartPos;
	loc.Y = DodgeValue;
	SetActorLocation(loc);
	GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &ABasicCharacter::ResetDodge, DodgeTimer, false);
}

void ABasicCharacter::ResetDodge()
{
	SetActorLocation(StartPos);
}

void ABasicCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsLunging = false;
}

void ABasicCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor->GetClass()->IsChildOf(AFallingActor::StaticClass()))
	{
		AFallingActor* Actor = Cast<AFallingActor>(OtherActor);

		if(Actor->GetType() == EObjectType::Bomb)
		{
			if(BombSound)
			{
				AudioComponent->SetSound(BombSound);
				AudioComponent->Play();
			}
		}
		else
		{
			if(MissedSound)
			{
				AudioComponent->SetSound(MissedSound);
				AudioComponent->Play();
			}
		}
		
		if(PreviousHitActor == nullptr)
		{
			GameModeRef->PlayerHit();
			PreviousHitActor = Actor;
		}
		
		if(PreviousHitActor != Actor)
		{
			if(Actor->GetType() == EObjectType::Bomb)
			{
				GameModeRef->GameOver("You Hit A bomb! Dodge the bombs");
			}
			if(Actor->GetType() == EObjectType::Ball)
			{
				
				GameModeRef->PlayerHit();
				
			}
		}
		PreviousHitActor = Actor;
	}
}
