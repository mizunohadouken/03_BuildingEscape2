// Project for fun

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Classes/Components/PrimitiveComponent.h"


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

	// ...
	Owner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll Triger Volume Every Frame
	if (GetTotalMassOfActorsOnPlate() > ActivationMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	float TimeSinceOpen = GetWorld()->GetTimeSeconds() - LastDoorOpenTime;
	if (TimeSinceOpen > DoorCloseDelay)
	{
		CloseDoor();
	}
}


void UOpenDoor::OpenDoor()
{
	// New rotation
	FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);

	// Set the door rotation
	Owner->SetActorRotation(NewRotation);
}

void UOpenDoor::CloseDoor()
{
	// Set the door closed rotation
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*>OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);
	for (auto* It : OverlappingActors)
	{
		TotalMass += It->FindComponentByClass<UPrimitiveComponent>()->CalculateMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *It->GetName())
	}

	return TotalMass;
}

