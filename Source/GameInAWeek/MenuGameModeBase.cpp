// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuGameModeBase.h"
#include "Blueprint/UserWidget.h"

AMenuGameModeBase::AMenuGameModeBase()
{
	
}

void AMenuGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	if(MenuClass)
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), MenuClass);
		if(Widget)
			Widget->AddToViewport();
	}
}
