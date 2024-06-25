

#include "CannonBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BallGates.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACannonBall::ACannonBall()
{
 	PrimaryActorTick.bCanEverTick = true;

	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphere"));
	SetRootComponent(RootSphere);

	RootSphere->SetNotifyRigidBodyCollision(true);

	CannonBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonBallMesh"));
	CannonBallMesh->SetupAttachment(GetRootComponent());

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;

	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 1.f;
	ProjectileMovement->Friction = 0.f;

	ProjectileMovement->ProjectileGravityScale = 0.f;

	ProjectileMovement->bConstrainToPlane = true;
	ProjectileMovement->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);

	// To avoid casting
	Tags.Add(FName("CannonBall"));
}

void ACannonBall::BeginPlay()
{
	Super::BeginPlay();

	RootSphere->OnComponentBeginOverlap.AddDynamic(this, &ACannonBall::OnBeginOverlap);
}

void ACannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACannonBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->ActorHasTag("Gates"))
		{
			Destroy();
		}
	}
}