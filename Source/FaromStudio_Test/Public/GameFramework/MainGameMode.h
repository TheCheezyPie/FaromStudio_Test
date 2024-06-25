// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FAROMSTUDIO_TEST_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()

//  Functions

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual bool ReadyToStartMatch_Implementation() override;

private:

//  Variables

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MainGameMode")
	int32 ReqPlayers = 2;

protected:

private:
	bool bFirstPlayer = true;
	
};
