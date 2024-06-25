// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

private:

//  Variables

public:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* RootSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* CannonBallMesh;

private:

};
