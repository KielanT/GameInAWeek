// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RhythmCharacter.generated.h"

UENUM()
enum class ELungeState
{
	//None = 0,
	Lunge = 0,
	HighLunge,
	LowLunge
};

UCLASS()
class GAMEINAWEEK_API ARhythmCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARhythmCharacter();

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

	void LowLunge();

	void HighLunge();

	void ResetLunge();

private:
	ELungeState LungeState;

	UPROPERTY(EditAnywhere, Category = Input)
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
		class UInputAction* LungeAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* HighLungeAction;

	UPROPERTY(EditAnywhere, Category = Input)
		UInputAction* LowLungeAction;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Category = Animation)
		UAnimMontage* LungeMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
		UAnimMontage* LungeLowMontage;

	UPROPERTY(EditAnywhere, Category = Animation)
		UAnimMontage* LungeHighMontage;
};
