// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "BasicPawn.generated.h"

UCLASS()
class GAMEINAWEEK_API ABasicPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasicPawn();

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
	
private:
	UPROPERTY(EditAnywhere, Category = Input)
		class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
		class UInputAction* MoveHorizontalAction;

	UPROPERTY(EditAnywhere)
		class UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(EditAnywhere)
		USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* TempStaticMesh;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* CameraComponent;
	
	
};
