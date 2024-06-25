// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Types.h"

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

	FORCEINLINE void SetTeam(ETeam NewTeam) { PlayerTeam = NewTeam; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Ugly. But maps are not supported in RPCs.
	UFUNCTION(Client, Reliable)
	void OnGatesHit(int32 FirstTeamScore, int32 SecondTeamScore);

	UFUNCTION()
	void OnGameStarted();

	UFUNCTION(Client, Reliable)
	void Client_OnGameStarted();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void Input_MovePlatform(float AxisValue);
	UFUNCTION(Server, unreliable, WithValidation)
	void Server_MovePlatform(float AxisValue);
	UFUNCTION(NetMulticast, unreliable)
	void Multicast_MovePlatform(float AxisValue);

private:
	void CreateMainUI();

	void CreateWaitingUI();
	void DestroyWaitingUI();

//  Variables

public:

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	ETeam PlayerTeam = ETeam::MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	float MoveSpeed = 100.0f;

	// To account for the spawn location of the platform
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	int32 Direction = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	TSubclassOf<class UMainUI> MainUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMainUI* MainUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	TSubclassOf<class UUserWidget> WaitingUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUserWidget* WaitingUI;

private:
	
};
