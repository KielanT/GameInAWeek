// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BasicCharacter.generated.h"

// All animations where created by me

class ASwordActor;
UCLASS()
class GAMEINAWEEK_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	
private:
	void Lunge();

	void RotateArm(const FInputActionValue& Value);

	void Pause();

	void SpawnSword();

	void DodgeRight();
	void DodgeLeft();
	void ResetDodge();

	void Dodge(float deltaTime);

	UFUNCTION()
		void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
public:
	UPROPERTY(BlueprintReadOnly)
		float ArmDeltaPitch;
	
private:
	UPROPERTY(EditAnywhere)
		USceneComponent* Root;

	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = Audio)
		USoundBase* BombSound;

	UPROPERTY(EditAnywhere, Category = Audio)
		USoundBase* MissedSound;
	
	UPROPERTY()
		class AGameInAWeekGameMode* GameModeRef;
	
	UPROPERTY(EditAnywhere, Category = Animation)
		UAnimMontage* LungeMontage;
	
	UPROPERTY(EditAnywhere, Category = Input)
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
		class UInputAction* MoveHorizontalAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* LungeAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* ArmAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* DodgeRightAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* DodgeLeftAction;
	
	UPROPERTY(EditAnywhere)
		class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = Input)
		float ArmMaxClamp = 90.0f;

	UPROPERTY(EditAnywhere, Category = Input)
		float ArmMinClamp = -90.0f;

	UPROPERTY(EditAnywhere, Category = Input)
		float ArmSpeed = 10.0f;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASwordActor> SwordActorClass;

	UPROPERTY()
		ASwordActor* SwordActor;

	UPROPERTY()
		FVector StartPos;

	UPROPERTY()
		FTimerHandle DodgeTimerHandle;

	UPROPERTY(EditAnywhere)
		float DodgeValue = 50.0f;
	
	
	UPROPERTY(EditAnywhere)
		float DodgeTimer = 1.0f;

	UPROPERTY(EditAnywhere)
		float DodgeSpeed = 10.0f;

	UPROPERTY()
		FVector DodgeDirection;
	
	UPROPERTY()
		class AFallingActor* PreviousHitActor;
	
	UPROPERTY()
		bool bIsActive;

	UPROPERTY()
		bool IsPaused;

	UPROPERTY()
		bool IsLunging;

	UPROPERTY()
		bool bIsDodging;

};

