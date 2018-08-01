// Copyright Rohan Paleja 2018

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Controller.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
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
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if physics handle is attached
	if (PhysicsHandle->GrabbedComponent) // if we grabbed something
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd()); // Places in target location
	}	 
}
void UGrabber::FindPhysicsHandleComponent()
{
	FString ObjectName = GetOwner()->GetName();
	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NO PHYSICS HANDLE FOUND FOR %s"), *ObjectName);
	}
}

void UGrabber::SetupInputComponent()
{
	FString ObjectName = GetOwner()->GetName();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release); // Can change to "Release" if you would like to hold
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("NO INPUT COMPONENT FOUND FOR %s"), *ObjectName);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyinReach()
{

	/*UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"),
	*PVPLocation.ToString(),
	*PVPRotation.ToString()
	)*/
	//Draw a red trace in the world to visualize
													//DrawDebugLine(
													//	GetWorld(),
													//	PVPLocation,
													//	LineTraceEnd,
													//	FColor(255, 0, 0),
													//	false,
													//	0.f,
													//	0.f,
													//	10.f
													//);
	/// Setup Query Param
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());  //This interacts with simple collision objects and ignore outself

	FHitResult HitResult;
	/// Ray-casting out to reach distance
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody)
	);
	// See what we hit
	AActor *ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		FString IntersectingActor = ActorHit->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Intersecting with: %s"), *IntersectingActor);
	}
	return HitResult;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Key Pressed"));

	/// Try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyinReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	/// if we hit something attach a physics handle
	if (ActorHit != nullptr)
	{
		PhysicsHandle->GrabComponent(
			ComponentToGrab, // turns out to be mesh
			NAME_None, // no bones needed
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true); // allow rotation
	}
}

void UGrabber::Release()
{
	/// UE_LOG(LogTemp, Warning, TEXT("Release Key Pressed"));
	// TODO Release physics handle
	PhysicsHandle->ReleaseComponent();
}

// Called every frame


FVector UGrabber::GetReachLineEnd()
{
	/// Get player viewpoint
	FVector PVPLocation;  //PVP is PlayerViewPoint
	FRotator PVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PVPLocation, OUT PVPRotation);


	FVector LineTrace = PVPRotation.Vector()*Reach;
	FVector LineTraceEnd = PVPLocation + LineTrace;
	return LineTraceEnd;
}

FVector UGrabber::GetReachLineStart()
{
	/// Get player viewpoint
	FVector PVPLocation;  //PVP is PlayerViewPoint
	FRotator PVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PVPLocation, OUT PVPRotation);
	return PVPLocation;
}