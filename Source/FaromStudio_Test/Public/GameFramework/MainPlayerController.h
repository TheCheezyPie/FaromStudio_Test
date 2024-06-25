// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FAROMSTUDIO_TEST_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

//  Functions

public:
	AMainPlayerController();
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE int32 GetDirection() const { return Direction; }

	UFUNCTION(Client, Reliable)
	void Client_AddTag(const FName& Tag);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void Input_MovePlatform(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MovePlatform(float AxisValue);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_MovePlatform(float AxisValue);

private:

//  Variables

public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	float MoveSpeed = 100.0f;

	// To account for the spawn location of the platform
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	int32 Direction = 1;

private:
	
};
