

#include "GameFramework/MainGameMode.h"
#include "GameFramework/MainPlayerController.h"

void AMainGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (AMainPlayerController* PC = StaticCast<AMainPlayerController*>(NewPlayer))
	{
		if (bFirstPlayer)
		{
			PC->Client_AddTag("First");
			//NewPlayer->Tags.Add("First");
			bFirstPlayer = false;
		}
		else
		{
			PC->Client_AddTag("Second");
			//NewPlayer->Tags.Add("Second");
		}
	}
}

bool AMainGameMode::ReadyToStartMatch_Implementation()
{
	return Super::ReadyToStartMatch_Implementation() && NumPlayers == ReqPlayers;
}
