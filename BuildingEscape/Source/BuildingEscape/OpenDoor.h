// Copyright Rohan Paleja 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	UPROPERTY(EditAnywhere) //VisibleAnywhere indicates is visible in all property windows but cannot be edited
	float OpenAngle = -90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;  // for storing time

	float LastDoorOpenTime;

	UPROPERTY(EditAnywhere)
	float MaxMassToOpenDoor = 20.f;  // for storing time
	
	AActor* Owner = nullptr; //The owning door

	// Returns total mass in kg
	float GetTotalMassofActorsOnPlate();
	void SetupPressurePlate();
	
};
