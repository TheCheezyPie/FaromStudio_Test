

#include "BallGates.h"
#include "Components/BoxComponent.h"

ABallGates::ABallGates()
{
 	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);

	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComponent->SetGenerateOverlapEvents(true);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	Tags.Add("Gates");
}

void ABallGates::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABallGates::OnBoxComponentBeginOverlap);
}

void ABallGates::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABallGates::OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("CannonBall"))
	{
		OnBallGatesOverlap.Broadcast(GateTeam);
	}
}
