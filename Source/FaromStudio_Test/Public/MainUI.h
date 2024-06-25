// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUI.generated.h"

/**
 * 
 */
UCLASS()
class FAROMSTUDIO_TEST_API UMainUI : public UUserWidget
{
	GENERATED_BODY()

//  Functions

public:
	void SetMyTeamScore(int32 Score);
	void SetEnemyTeamScore(int32 Score);

protected:

private:

//  Variables

public:

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* MyTeamScore;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* EnemyTeamScore;
private:
};
