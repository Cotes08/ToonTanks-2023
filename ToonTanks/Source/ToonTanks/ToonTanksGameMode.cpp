// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor)
{   
    if (DeadActor == Tank)
    {
        //Llamamos al metodo de destruccion del tanque
        Tank->HandleDestruction();
        //Comprobamos que el controlador del tanque no sea nulo y si no lo es desactivamos los inputs y mostramos el cursor
        if (ToonTanksPlayerController)
        {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    }else if (ATower* DestroyedTower = Cast<ATower>(DeadActor)) 
    {
        //Si el actor que ha muerto es una torre llamamos a su metodo de destruccion
        DestroyedTower->HandleDestruction();

        TargetTowersCount--;

        if (TargetTowersCount == 0) { GameOver(true);}
    }
    
}

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();

}


void AToonTanksGameMode::HandleGameStart()
{
    TargetTowersCount = GetTargetTowersCount();

    //Creamos una variable de tipo ATank y le asignamos el valor del tanque que tenemos en el mundo
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    //Creamos una variable de tipo AToonTanksPlayerController y le asignamos el valor del controlador del tanque que tenemos en el mundo
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController)
    {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableTimerHandle;

        //El timer delegate esta vinculado a SetPlayerEnabledState, por lo que cuando se cumpla el tiempo se llamara a SetPlayerEnabledState con el parametro true
        FTimerDelegate PlayerEnableTimerHandleDelegate = FTimerDelegate::CreateUObject(
            ToonTanksPlayerController, 
            &AToonTanksPlayerController::SetPlayerEnabledState,
            true
        );

        //Llamamos al metodo SetTimer de la clase WorldTimerManager para que se ejecute el timer delegate pasados los segundos de StartDelay
        //Con esto logramos que el juego empiece tras 3 segundos
        GetWorldTimerManager().SetTimer(
            PlayerEnableTimerHandle, 
            PlayerEnableTimerHandleDelegate, 
            StartDelay, 
            false
        );
    }
}

int32 AToonTanksGameMode::GetTargetTowersCount()
{
    TArray<AActor*> Towers;

    //Cogemos todos los actores del mundo(this), del tipo tower(TSubclassOf) y lo almacenamos en un array de actores(TArray<AActor*>)
    UGameplayStatics::GetAllActorsOfClass(this, TSubclassOf<class ATower>(ATower::StaticClass()), Towers);
    return Towers.Num();
}