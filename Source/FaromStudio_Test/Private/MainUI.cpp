// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"
#include "Components/TextBlock.h"

void UMainUI::SetMyTeamScore(int32 Score)
{
	MyTeamScore->SetText(FText::FromString(FString::FromInt(Score)));
}

void UMainUI::SetEnemyTeamScore(int32 Score)
{
	EnemyTeamScore->SetText(FText::FromString(FString::FromInt(Score)));
}
