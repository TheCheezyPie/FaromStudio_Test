

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

void AMainPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	Direction = aPawn->GetActorLocation().Y > 0 ? 1 : -1;
}

void AMainPlayerController::OnGameStarted()
{
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
	DestroyWaitingUI();
	CreateMainUI();
}

void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AMainGameMode* GM = GetWorld()->GetAuthGameMode<AMainGameMode>())
		{
			GM->OnGameStarted.AddDynamic(this, &AMainPlayerController::OnGameStarted);
		}
	}
	else
	{
		// To make sure the waiting UI is not created in case of 
		// a desync between the server and the client
		if (!MainUI)
		{
			CreateWaitingUI();
		}
	}
}

void AMainPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleActionsMovement(DeltaTime);
}

void AMainPlayerController::HandleActionsMovement(float DeltaTime)
{
	if (HasAuthority() && FMath::Abs(MovementActionValue) > 0.01f)
	{
		if (APawn* PPawn = GetPawn())
		{
			PPawn->AddActorWorldOffset(FVector(MovementActionValue * MoveSpeed * DeltaTime * 3.f, 0, 0) * GetDirection(), true);
		}
	}
}

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("Move", this, &AMainPlayerController::Input_MovePlatform);

	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &AMainPlayerController::Input_MoveLeft);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &AMainPlayerController::Input_MoveRight);

	InputComponent->BindAction("MoveLeft", IE_Released, this, &AMainPlayerController::Stop_MoveLeft);
	InputComponent->BindAction("MoveRight", IE_Released, this, &AMainPlayerController::Stop_MoveRight);
}

void AMainPlayerController::Input_MoveLeft_Implementation()
{
	MovementActionValue = -1.0f;
}

void AMainPlayerController::Input_MoveRight_Implementation()
{
	MovementActionValue = 1.0f;
}

void AMainPlayerController::Stop_MoveLeft_Implementation()
{
	if (MovementActionValue < 0)
	{
		MovementActionValue = 0;
	}
}

void AMainPlayerController::Stop_MoveRight_Implementation()
{
	if (MovementActionValue > 0)
	{
		MovementActionValue = 0;
	}
}

void AMainPlayerController::Input_MovePlatform(float AxisValue)
{
	// To avoid RPCs every tick
	// They still are techincally called ON tick
	// but the value is checked to avoid EVERY tick
	if (FMath::Abs(AxisValue) > 0.01f)
	{
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
	if (WaitingUI)
	{
		WaitingUI->RemoveFromParent();
	}
}

void AMainPlayerController::Server_MovePlatform_Implementation(float AxisValue)
{
	if (APawn* PPawn = GetPawn())
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		FVector DeltaLocation = FVector(AxisValue * MoveSpeed * DeltaTime, 0, 0) * GetDirection();

		PPawn->AddActorWorldOffset(DeltaLocation, true);
	}
}