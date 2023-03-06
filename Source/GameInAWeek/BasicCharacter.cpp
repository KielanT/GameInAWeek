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

	// Sets the Root Component to a scene component instead of the Capsule so the camera does not move with it
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// Initilises and Attaches the capsule to the Root
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetupAttachment(Root);

	// Creates Camera 
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	//CameraComponent->SetupAttachment(Root);

	// Creates the audio component used for playing sounds when the player is hit
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->SetupAttachment(GetCapsuleComponent());

}

// Called when the game starts or when spawned
void ABasicCharacter::BeginPlay()
{
	Super::BeginPlay();
	AudioComponent->Stop(); // Disable the audio component

	GameModeRef = Cast<AGameInAWeekGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Set up the Input Mapping Context
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* SubsystemInterface = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			SubsystemInterface->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Spawns the selected sword
	SpawnSword();

	// Binds the on hit function
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABasicCharacter::OnHit);

	// Sets starting values
	IsPaused = false;
	IsLunging = false;
	bIsDodging = false;
	PreviousHitActor = nullptr;
}

// Called every frame
void ABasicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bIsActive = GameModeRef->IsPlaying(); // Variable used to check if the game is playing
	SwordActor->bIsPlayerLunging = IsLunging; // Allows the sword to know if the player is lunging

	// Enables dodge movement when the player dodges
	if(bIsDodging)
		Dodge(DeltaTime);
}

// Called to bind functionality to input
void ABasicCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setups the Input Actions
	if(UEnhancedInputComponent* InputComp = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
		if(LungeAction) // Default lunge is left mouse
		{
			InputComp->BindAction(LungeAction, ETriggerEvent::Triggered, this, &ABasicCharacter::Lunge);
		}
		if(ArmAction) // Default Arm is Mouse Y
		{
			InputComp->BindAction(ArmAction, ETriggerEvent::Triggered, this, &ABasicCharacter::RotateArm);
		}
		if(PauseAction) // Default Pause is Escape or P
		{
			InputComp->BindAction(PauseAction, ETriggerEvent::Started, this, &ABasicCharacter::Pause);
		}
		if(DodgeLeftAction) // Default Dodge Left is S
		{
			InputComp->BindAction(DodgeLeftAction, ETriggerEvent::Completed, this, &ABasicCharacter::DodgeLeft);
		}
		if(DodgeRightAction) // Default Dodge Right is W
		{
			InputComp->BindAction(DodgeRightAction, ETriggerEvent::Completed, this, &ABasicCharacter::DodgeRight);
		}
    }

}


void ABasicCharacter::Lunge()
{
	// Only can lunge if the game is in play state
	if(bIsActive)
	{
		// Peforms lunge if the montage is not running and exists
		if(LungeMontage && GetCurrentMontage() != LungeMontage) // Detecting the montage playing stops the player spamming
		{
			IsLunging = true;

			// Uses AnimInstance rather than the PlayAnimMontage Function to be able to use OnMontageEnd
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
	if(bIsActive) // Only rotates the arm if the game is in play state
	{
		float input = Value.Get<float>() * ArmSpeed; // Gets the input and multiplies it by the speed

		// Sets the Arm Delta to be used in the animation blueprint to rotate the arm
		// Clamps between the min and max to stop it from rotating a full 360
		ArmDeltaPitch = FMath::Clamp(input + ArmDeltaPitch, ArmMinClamp, ArmMaxClamp); 
	}
}

void ABasicCharacter::Pause()
{
	// Allows Pausing to change with a single button press
	IsPaused = !IsPaused;

	// Pauses or unpauses depending on true or false
	IsPaused ? GameModeRef->Pause() : GameModeRef->Resume();
}

void ABasicCharacter::SpawnSword()
{
	// Spawns sword if the class exists (Set in BP)
	if(SwordActorClass) 
	{
		FActorSpawnParameters Params; // Default Spawn Parameters

		// Sets the attachment rules, this snaps to hand and stops the sword from scaling
		FAttachmentTransformRules Rules = FAttachmentTransformRules::SnapToTargetNotIncludingScale; 

		Rules.RotationRule = EAttachmentRule::KeepWorld; // Keeps its Rotation Otherwise it's rotation is wrong
		
		SwordActor = GetWorld()->SpawnActor<ASwordActor>(SwordActorClass, GetTransform(), Params); // Spawns the sword
		SwordActor->AttachToComponent(GetMesh(), Rules, FName("WeaponSocket")); // Attaches the sword to the hand (custom socket made to make this easier)
	}
}

void ABasicCharacter::DodgeRight()
{
	// Sets the start pos for resetting
	StartPos = GetActorLocation();

	// If not already dodging then sets up the dodge direction and starts the timer (dodge happens in the dodge function found in tick)
	if(!bIsDodging) 
	{
		bIsDodging = true;
		DodgeDirection = -FVector::YAxisVector;
		GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &ABasicCharacter::ResetDodge, DodgeTimer, false);
	}
	else
	{
		// Resets dodge Allows the button to be pressed again to reset
		SetActorLocation(StartPos); 
		bIsDodging = false;
	}
}

void ABasicCharacter::DodgeLeft()
{
	// Sets the start pos for resetting
	StartPos = GetActorLocation();

	// If not already dodging then sets up the dodge direction and starts the timer (dodge happens in the dodge function found in tick)
	if(!bIsDodging)
	{
		bIsDodging = true;
		DodgeDirection = FVector::YAxisVector;

		GetWorld()->GetTimerManager().SetTimer(DodgeTimerHandle, this, &ABasicCharacter::ResetDodge, DodgeTimer, false);
	}
	else
	{
		// Resets dodge Allows the button to be pressed again to reset
		SetActorLocation(StartPos);
		bIsDodging = false;
	}
}

void ABasicCharacter::ResetDodge()
{
	// Resets dodge 
	SetActorLocation(StartPos);
	bIsDodging = false;
}

void ABasicCharacter::Dodge(float deltaTime)
{
	
	FVector Loc = GetActorLocation(); // Sets the X and Z positions to be correct

	// Moves the actor to a new position to dodge depending on the direction set
	if(DodgeDirection.Y > 0.0f) 
	{
		Loc.Y = DodgeValue;
		if(Loc.Y >= GetActorLocation().Y)
		{
			AddActorWorldOffset(DodgeDirection * DodgeSpeed);
		}
	}
	if (DodgeDirection.Y < 0.0f)
	{
		Loc.Y = -DodgeValue;
		if(Loc.Y <= GetActorLocation().Y)
		{
			AddActorWorldOffset(DodgeDirection * DodgeSpeed);
		}
	}
	

}

void ABasicCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsLunging = false; // Used to tell the sword that it's stopped lunging
}

void ABasicCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// If the playing is not dodge then Player gets hit
	if(!bIsDodging)
	{
		if(OtherActor->GetClass()->IsChildOf(AFallingActor::StaticClass())) // Makes sure that the object is of correct type
		{
			AFallingActor* Actor = Cast<AFallingActor>(OtherActor);

			// If the actor is a bomb play the bomb sound if not play the miss sound
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

			// If there is no previous actor still allows a hit 
			if(PreviousHitActor == nullptr)
			{
				GameModeRef->PlayerHit();
				PreviousHitActor = Actor;
			}

			// This make sure that the player only detects being hit once by each object since it
			// Can keep hitting the player
			if(PreviousHitActor != Actor) 
			{
				if(Actor->GetType() == EObjectType::Bomb) // If the actor is a bomb end the game
				{
					GameModeRef->GameOver("You Hit A bomb! Dodge the bombs"); // Sets game over with a reason
				}
				if(Actor->GetType() == EObjectType::Ball) // Just calls hit if its not a bomb
				{
					GameModeRef->PlayerHit();
				
				}
			}
			PreviousHitActor = Actor; // Sets the previous actor for checking
		}
	}
}
