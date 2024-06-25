// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types.h"

#include "CannonBall.generated.h"

UCLASS()
class FAROMSTUDIO_TEST_API ACannonBall : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannonBall();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

//  Functions

public:

protected:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

//  Variables

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* RootSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* CannonBallMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector MoveDirection{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 400.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float StartZ = 0.f;

private:

};
