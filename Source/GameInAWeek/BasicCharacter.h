// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BasicCharacter.generated.h"


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
	void Move(const FInputActionValue& Value);

	void Lunge();

	void Retreat();

	void RotateArm(const FInputActionValue& Value);

	void SpawnSword();

public:
	UPROPERTY(BlueprintReadWrite)
		bool bIsLunge = false;

	UPROPERTY(BlueprintReadWrite)
		bool bIsRetreat = false;

	UPROPERTY(BlueprintReadOnly)
		float ArmDeltaPitch;
	
private:
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
		FRotator StartingRotation;
};
