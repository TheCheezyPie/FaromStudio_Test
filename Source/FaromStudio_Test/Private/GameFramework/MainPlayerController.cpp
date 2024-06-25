

#include "GameFramework/MainPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "BallGates.h"
#include "MainUI.h"
#include "GameFramework/MainGameMode.h"

AMainPlayerController::AMainPlayerController()
{
}

void AMainPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMainPlayerController, PlayerTeam);
}

void AMainPlayerController::OnGameStarted()
{
	LOG("OnGameStarted");
	Client_OnGameStarted();
}

void AMainPlayerController::OnGatesHit_Implementation(int32 FirstTeamScore, int32 SecondTeamScore)
{
	if (MainUI)
	{
		switch (PlayerTeam)
		{
		case ETeam::First:
			MainUI->SetMyTeamScore(FirstTeamScore);
			MainUI->SetEnemyTeamScore(SecondTeamScore);
			break;
		case ETeam::Second:
			MainUI->SetMyTeamScore(SecondTeamScore);
			MainUI->SetEnemyTeamScore(FirstTeamScore);
			break;
		}
	}
}

void AMainPlayerController::Client_OnGameStarted_Implementation()
{
	LOG("Client_OnGameStarted");
	DestroyWaitingUI();
	CreateMainUI();
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// For some reason one of the clients' direction changes chaotically,
	// so I decided to set the direction in the MovePlatform function
	// (essentially every frame, which is BAD, I know)
	//if (APawn* PPawn = GetPawn())
	//{
	//	Direction = PPawn->GetActorLocation().Y > 0 ? 1 : -1;
	//}

	if (HasAuthority())
	{
		if (AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>())
		{
			GM->OnGameStarted.AddDynamic(this, &AMainPlayerController::OnGameStarted);
		}
	}
	else
	{
		if (!MainUI)
		{
			CreateWaitingUI();
		}
	}
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

void AMainPlayerController::CreateMainUI()
{
	if (MainUIClass)
	{
		MainUI = CreateWidget<UMainUI>(this, MainUIClass);
		if (MainUI)
		{
			MainUI->SetMyTeamScore(0);
			MainUI->SetEnemyTeamScore(0);

			MainUI->AddToViewport();
		}
	}
}

void AMainPlayerController::CreateWaitingUI()
{
	if (WaitingUIClass)
	{
		WaitingUI = CreateWidget<UUserWidget>(this, WaitingUIClass);
		if (WaitingUI)
		{
			WaitingUI->AddToViewport();
		}
	}
}

void AMainPlayerController::DestroyWaitingUI()
{
	LOG("Destroying Waiting UI")
	if (WaitingUI)
	{
		WaitingUI->RemoveFromParent();
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