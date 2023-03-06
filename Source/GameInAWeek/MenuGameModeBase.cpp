// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameModeBase.h"
#include "Blueprint/UserWidget.h"



AMenuGameModeBase::AMenuGameModeBase()
{
	
}

void AMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	// Gamemode for the menu just to add it to viewport
	if(MenuClass)
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), MenuClass);
		if(Widget)
		{
			Widget->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
	}
}
