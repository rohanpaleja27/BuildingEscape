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
	SetupPressurePlate();
}






// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the Trigger Volume 
	if (GetTotalMassofActorsOnPlate() > MaxMassToOpenDoor) 
	{
		// If the ActorThatOpens is in the volume
		OnOpen.Broadcast();
	}
	// Check if its time to close the door
	else
	{
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassofActorsOnPlate()
{
	float TotalMass = 0.f;
	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate)	{ return TotalMass; } 
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);// Fills this tarray
	 //iterate through their masses and add
	for (const auto* Actor : OverlappingActors)
	{
		
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s intersecting with pressure plate"), *Actor->GetName());
	}
	return TotalMass;
}

void UOpenDoor::SetupPressurePlate()
{
	if (!PressurePlate)
	{
		FString ObjectName = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("Missing Pressure Plate for %s"), *ObjectName);
	}
	return;
}

