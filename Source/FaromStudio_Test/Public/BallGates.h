
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types.h"

#include "BallGates.generated.h"

// When the ball enters the gate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBallGatesOverlap, ETeam, Team);

UCLASS()
class FAROMSTUDIO_TEST_API ABallGates : public AActor
{
	GENERATED_BODY()
	
public:	
	ABallGates();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

//  Functions

public:

protected:
	UFUNCTION()
	void OnBoxComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

//  Variables

public:
	FOnBallGatesOverlap OnBallGatesOverlap;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETeam GateTeam = ETeam::First;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComponent;

private:

};
