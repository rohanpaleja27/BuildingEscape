// Copyright Rohan Paleja 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent); // Makes a new class

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


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnClose;

private:
	

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;


	UPROPERTY(EditAnywhere)
	float MaxMassToOpenDoor = 20.f;  // for storing time
	
	AActor* Owner = nullptr; //The owning door

	// Returns total mass in kg
	float GetTotalMassofActorsOnPlate();
	void SetupPressurePlate();
	
};
