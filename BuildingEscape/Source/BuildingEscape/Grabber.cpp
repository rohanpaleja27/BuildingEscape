// Copyright Rohan Paleja 2018

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"
#include "GameFramework/Actor.h"

#define OUT  //MACRO does nothing, compiler subsitutes OUT for nothing
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FString ObjectName = GetOwner()->GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s is reporting for duty"), *ObjectName);
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player viewpoint
	FVector PVPLocation;  //PVP is PlayerViewPoint
	FRotator PVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PVPLocation,OUT PVPRotation);
	
	
	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), 
		*PVPLocation.ToString(), 
		*PVPRotation.ToString()
	)*/
	FVector LineTrace = PVPRotation.Vector()*Reach;
	FVector LineTraceEnd = PVPLocation +LineTrace; //cm
	//Draw a red trace in the world to visualize
	DrawDebugLine(
		GetWorld(),
		PVPLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f
	);
	// Ray-casting out to reach distance
	// See what we hit
}

