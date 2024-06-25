#pragma once

#include "CoreMinimal.h"

#define LOG(Format, ...) UE_LOG(LogTemp, Warning, TEXT(Format), ##__VA_ARGS__)
#define SCREEN_LOG(Format, ...) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT(Format), ##__VA_ARGS__))

UENUM(BlueprintType)
enum class ETeam : uint8
{
	First,
	Second,

	MAX
};