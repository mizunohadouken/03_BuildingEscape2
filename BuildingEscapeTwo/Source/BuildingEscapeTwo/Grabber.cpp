// Project for fun

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Grabber.h"


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

	// ...
	object_name = GetOwner()->GetName();

	FindPhysicsHandleComponent();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	/// Look for attached InputComponent
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		/// Bind input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UInputComponent found on %s"), *object_name);
	}
}


/// Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		// PhysicsHandle found
		UE_LOG(LogTemp, Warning, TEXT("UPhysicsHandleComponent found on %s"), *object_name);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No UPhysicsHandleComponent found on %s"), *object_name);
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// if the physics handle is attached
		// move the object that we're holding
	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Button Pressed"));

	// Line trace and try and reach any actors with physics body collision channel set 
	GetFirstPhysicsBodyInReach();

	// If we hit then attach a physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Button Released"));

	// Release physics handle
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get player view point this tick
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

	FVector LineTraceEnd = PlayerViewLocation + PlayerViewRotation.Vector() * Reach;

	// Draw a blue trace in world
	//	DrawDebugLine(GetWorld(), PlayerViewLocation, LineTraceEnd,	FColor(0, 0, 255), false,0.0f, 0.0f, 10.0f);

	/// Ray-cast out to reach distance
	FHitResult hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(hit,
		PlayerViewLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams);

	// See what we hit
	AActor* ActorHit = hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s has been hit"), *ActorHit->GetName());
	}

	return FHitResult();
}
