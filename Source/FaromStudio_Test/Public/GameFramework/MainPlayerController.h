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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Gets called on the SERVER
	virtual void OnPossess(APawn* aPawn) override;

	// Ugly. But maps are not supported in the RPCs.
	UFUNCTION(Client, Reliable)
	void OnGatesHit(int32 FirstTeamScore, int32 SecondTeamScore);

	UFUNCTION()
	void OnGameStarted();

	// Client version of OnGameStarted to handle UI 
	// (which is only on the client side)
	UFUNCTION(Client, Reliable)
	void Client_OnGameStarted();

	FORCEINLINE int32 GetDirection() const { return Direction; }

	FORCEINLINE void SetTeam(ETeam NewTeam) { PlayerTeam = NewTeam; }

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	// Functions to workaroud the movement RPCs on tick
	// Only triggered by ActionMovement (A, D) (Press)
		UFUNCTION(Server, Reliable)
		void Input_MoveLeft();
		UFUNCTION(Server, Reliable)
		void Input_MoveRight();
	//

	// Functions to workaroud the movement RPCs on tick
	// Only triggered by ActionMovement (A, D) (Release)
		UFUNCTION(Server, Reliable)
		void Stop_MoveLeft();
		UFUNCTION(Server, Reliable)
		void Stop_MoveRight();
	//

	// Called when the player presses the movement button
	// On the client side
	void Input_MovePlatform(float AxisValue);

	// Server version of Input_MovePlatform
	// to handle movement for all players
	// Unreliable because it's called on tick
	// and one or two lost packets are not a big deal (in this case)
	UFUNCTION(Server, unreliable)
	void Server_MovePlatform(float AxisValue);

private:
	// AND add to viewport
	void CreateMainUI();

	// AND add to viewport
	void CreateWaitingUI();
	void DestroyWaitingUI();

	// Called every every frame to avoid RPCs on tick
	// Only handles the ActionMovement (A, D)
	void HandleActionsMovement(float DeltaTime);

//  Variables

public:

protected:
	// Part of a workaround to avoid RPCs every tick
	// A is -1, D is 1
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MovementActionValue = 0.0f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly)
	ETeam PlayerTeam = ETeam::MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	float MoveSpeed = 100.0f;

	// To account for the spawn location of the platform
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	int32 Direction = 1;

	// Class to spawn the main UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	TSubclassOf<class UMainUI> MainUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMainUI* MainUI;
	
	// Class to spawn the waiting UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainPlayerController")
	TSubclassOf<class UUserWidget> WaitingUIClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UUserWidget* WaitingUI;

private:
	
};
