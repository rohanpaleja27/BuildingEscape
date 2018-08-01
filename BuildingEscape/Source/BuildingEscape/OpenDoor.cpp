// Copyright Rohan Paleja 2018

#include "OpenDoor.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#define OUT // out parameters

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner(); //door actor
	
}

void UOpenDoor::OpenDoor()
{
	FRotator opener = FRotator(0.0f, OpenAngle, 0.0f);	
	Owner->SetActorRotation(opener);
}

void UOpenDoor::CloseDoor()
{
	FRotator closer = FRotator(0.0f, 0.0f, 0.0f);
	Owner->SetActorRotation(closer);
	/*
	Troubleshooting code
	FString Rotation = Owner->GetActorRotation().ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s is at"), *Rotation);
	*/
}




// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume 
	if (GetTotalMassofActorsOnPlate() > 20.f) // TODO make into a private variable
	{
		// If the ActorThatOpens is in the volume
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	// Check if its time to close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime >= DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassofActorsOnPlate()
{
	float TotalMass = 0.f;
	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors); // Fills this tarray
	 //iterate through their masses and add
	for (const auto* Actor : OverlappingActors)
	{
		
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s intersecting with pressure plate"), *Actor->GetName());
	}
	return TotalMass;
}

