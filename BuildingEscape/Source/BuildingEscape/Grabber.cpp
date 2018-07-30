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
	
	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// Physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO PHYSICS HANDLE FOUND FOR %s"), *ObjectName);
	}
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		// Input Component is found
		// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO INPUT COMPONENT FOUND FOR %s"), *ObjectName);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Key Pressed"));
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get player viewpoint
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
	/// Setup Query Param
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());  //This interacts with simple collision objects and ignore outself

	FHitResult Hit;
	/// Ray-casting out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PVPLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody)
		);
	// See what we hit
	AActor *ActorHit = Hit.GetActor();
	if (ActorHit)
	{
	FString IntersectingActor = ActorHit->GetName();
	UE_LOG(LogTemp, Warning, TEXT("Intersecting with: %s"), *IntersectingActor);
	}
	 
}

