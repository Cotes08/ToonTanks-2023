// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
//#include "DrawDebugHelpers.h"

ATank::ATank()
{

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    //Llamamos al padre para que se encargue de la configuracion de los inputs
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    //El this es la clase tanque, el &ATank::Move es el metodo (Move) que queremos llamar
    //El texto es MoveForward porque es el nombre que le hemos dado en el proyecto en axis input
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

    //Como es solo pulsar un boton y no movimiento es Action y no Axis
    PlayerInputComponent->BindAction(TEXT("FIRE"), IE_Pressed, this, &ATank::Fire);

};

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (TankPlayerController)
    {   
        FHitResult HitResult;

        TankPlayerController->GetHitResultUnderCursor(
            ECollisionChannel::ECC_Visibility,
            false,
            HitResult
        );

        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12, FColor::Red, false, -1.f);
        RotateTurret(HitResult.Location);
    }  
}

void ATank::HandleDestruction()
{
    //Llamamos al padre para obtener el efecto de destruccion
    Super::HandleDestruction();
    //En este caso solo queremos que el tanque desaparezca para que se quede la camara en el lugar de la muerte
    SetActorHiddenInGame(true);
    //Desactivamos el tick para que no se siga ejecutando el codigo
    SetActorTickEnabled(false);
    bAlive = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

    //Sirve para cambiar el tipo de controller a un player controller, solo se puede hacer si pertenecen a la misma clase
    TankPlayerController = Cast<APlayerController>(GetController());
	
}

void ATank::Move(float Value)
{
    //Declaramos un vactor y lo inicializamos a 0
    FVector DeltaLocation = FVector::ZeroVector;
    //Le damos el valor a la x para que varie segun el input
    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
    //Aplicamos ese movimiento al tanque
    AddActorLocalOffset(DeltaLocation, true);

};


void ATank::Turn(float Value)
{
    //Declaramos un Rotator y lo inicializamos a 0
    FRotator DeltaRotation = FRotator::ZeroRotator;
    //Le damos el valor a yaw para que varie segun el input
    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
    //Aplicamos ese movimiento al tanque
    AddActorLocalRotation(DeltaRotation, true);

};
