

#include "GameFramework/MainPlayerController.h"
#include "Net\UnrealNetwork.h"

AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::Client_AddTag_Implementation(const FName& Tag)
{
	Tags.Add(Tag);
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// For some reason one of the clients' direction changes chaotically,
	// so I decided to set the direction in the MovePlatform function
	//if (APawn* PPawn = GetPawn())
	//{
	//	Direction = PPawn->GetActorLocation().Y > 0 ? 1 : -1;
	//}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Move", this, &AMainPlayerController::Input_MovePlatform);
}

void AMainPlayerController::Input_MovePlatform(float AxisValue)
{
	if (FMath::Abs(AxisValue) > 0.01f)
	{
		if (Tags.IsValidIndex(0))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Tags[0].ToString());
		}
		Server_MovePlatform(AxisValue);
	}
}

void AMainPlayerController::Server_MovePlatform_Implementation(float AxisValue)
{
	Multicast_MovePlatform(AxisValue);
}

bool AMainPlayerController::Server_MovePlatform_Validate(float AxisValue)
{
	return true;
}

void AMainPlayerController::Multicast_MovePlatform_Implementation(float AxisValue)
{
	if (APawn* PPawn = GetPawn())
	{
		Direction = PPawn->GetActorLocation().Y > 0 ? 1 : -1;
		PPawn->AddActorWorldOffset(FVector(AxisValue * MoveSpeed, 0, 0) * GetDirection(), true);
	}
}