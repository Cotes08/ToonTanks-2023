// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    if (bPlayerEnabled)
    {
        //Como es una clase de tipo player controller, podemos acceder a la variable player pawn 
        GetPawn()->EnableInput(this);
    }else{
        GetPawn()->DisableInput(this);
    }

    //Mostramos el cursor (bShowMouseCursor es un metodo de la clase player controller)
    bShowMouseCursor = bPlayerEnabled;
    
}

