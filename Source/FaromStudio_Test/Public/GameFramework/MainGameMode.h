// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FAROMSTUDIO_TEST_API AMainGameMode : public AGameModeBase
{
	GENERATED_BODY()

//  Functions

public:
	virtual void BeginPlay() override;

	virtual void StartMatch() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	UFUNCTION()
	void OnGatesHit(ETeam Team);

protected:
	virtual bool ReadyToStartMatch_Implementation() override;

	void SpawnBall();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnBall();

	UFUNCTION()
	void SpawnBall_Delegate(AActor* DestroyedActor);

private:

//  Variables

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainGameMode")
	int32 ReqPlayers = 2;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainGameMode")
	TSubclassOf<class AActor> SpawnPointClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainGameMode")
	TSubclassOf<class ACannonBall> BallClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MainGameMode")
	TMap<ETeam, int32> Score;

private:
	bool bFirstPlayer = true;
	
};
