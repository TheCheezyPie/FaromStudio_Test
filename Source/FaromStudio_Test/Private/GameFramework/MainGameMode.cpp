

#include "GameFramework/MainGameMode.h"
#include "GameFramework/MainPlayerController.h"
#include "CannonBall.h"
#include "BallGates.h"
#include "Kismet/GameplayStatics.h"

void AMainGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Subscribe to gates' delegates
	TArray<AActor*> GatesArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABallGates::StaticClass(), GatesArray);

	for (AActor* Gate : GatesArray)
	{
		ABallGates* BallGate = StaticCast<ABallGates*>(Gate);
		if (BallGate)
		{
			BallGate->OnBallGatesOverlap.AddDynamic(this, &AMainGameMode::OnGatesHit);
		}
	}
	// Subscribe to gates' delegates
}

void AMainGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bGameStarted && ReadyToStartMatch())
	{
		StartMatch();
		bGameStarted = true;
	}
}

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer)
	{
		NumPlayers++;
	}
}

AActor* AMainGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	AActor* PlayerStart = nullptr;

	if (NumPlayers == 0)
	{
		PlayerStart = FindPlayerStart(Player, "First");
		if (PlayerStart)
		{
			AMainPlayerController* PC = StaticCast<AMainPlayerController*>(Player);
			PC->SetTeam(ETeam::First);
		}
	}
	else
	{
		PlayerStart = FindPlayerStart(Player, "Second");
		if (PlayerStart)
		{
			AMainPlayerController* PC = StaticCast<AMainPlayerController*>(Player);
			PC->SetTeam(ETeam::Second);
		}
	}

	return PlayerStart;
}

void AMainGameMode::OnGatesHit(ETeam Team)
{
	if (Score.Contains(Team))
	{
		switch (Team)
		{
		case ETeam::First:
			Score[ETeam::Second]++;
			break;
		case ETeam::Second:
			Score[ETeam::First]++;
			break;
		}
	}

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (AMainPlayerController* PC = StaticCast<AMainPlayerController*>(It->Get()))
		{
			PC->OnGatesHit(Score[ETeam::First], Score[ETeam::Second]);
		}
	}
}

void AMainGameMode::StartMatch()
{
	Score.Add(ETeam::First, 0);
	Score.Add(ETeam::Second, 0);

	SpawnBall();
	OnGameStarted.Broadcast();
}

bool AMainGameMode::ReadyToStartMatch()
{
	return NumPlayers == ReqPlayers;
}

void AMainGameMode::SpawnBall()
{
	if (SpawnPointClass)
	{
		TArray<AActor*> Array;
		UGameplayStatics::GetAllActorsOfClass(this, SpawnPointClass, Array);
		
		if (Array.Num() == 0) return;

		AActor* BallSpawnPoint = Array[0];

		if (BallSpawnPoint && BallClass)
		{
			FRotator RandomDirection = FRotator(0.f, FMath::RandRange(-180.f, 180.f), 0.f);

			ACannonBall* Ball = GetWorld()->SpawnActor<ACannonBall>(BallClass, BallSpawnPoint->GetActorLocation(), RandomDirection);
			if (Ball)
			{
				Ball->OnDestroyed.AddDynamic(this, &AMainGameMode::SpawnBall_Delegate);
			}
		}
	}
}

void AMainGameMode::SpawnBall_Delegate(AActor* DestroyedActor)
{
	SpawnBall();
}
