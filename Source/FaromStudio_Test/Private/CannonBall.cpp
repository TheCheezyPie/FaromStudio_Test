

#include "CannonBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

ACannonBall::ACannonBall()
{
 	PrimaryActorTick.bCanEverTick = true;

	RootSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphere"));
	SetRootComponent(RootSphere);
	RootSphere->SetNotifyRigidBodyCollision(true);

	CannonBallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonBallMesh"));
	CannonBallMesh->SetupAttachment(GetRootComponent());
}

void ACannonBall::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

