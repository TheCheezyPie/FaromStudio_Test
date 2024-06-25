// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStarted);

UCLASS()
class FAROMSTUDIO_TEST_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

//  Functions

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	// Called when the ball hits the gates
	UFUNCTION()
	void OnGatesHit(ETeam Team);

protected:
	// Notify the controllers, nullify scores, spawn a ball
	void StartMatch();
	// Check whether the game can be started
	bool ReadyToStartMatch();

	void SpawnBall();

	// A version of SpawnBall() that can be tied to an actor's OnDestroyed event
	// @param DestroyedActor - Placeholder for the delegate
	UFUNCTION()
	void SpawnBall_Delegate(AActor* DestroyedActor);

private:

//  Variables

public:
	FOnGameStarted OnGameStarted;

protected:
	// To start the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainGameMode")
	int32 ReqPlayers = 2;

	// Class to find an actor to act as a spawn point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainGameMode")
	TSubclassOf<class AActor> SpawnPointClass;

	// Class to spawn a ball
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainGameMode")
	TSubclassOf<class ACannonBall> BallClass;

	// Teams' scores
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainGameMode")
	TMap<ETeam, int32> Score;

	// Current number of players
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainGameMode")
	int NumPlayers = 0;

	// Game started flag
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainGameMode")
	bool bGameStarted = false;

private:
};
