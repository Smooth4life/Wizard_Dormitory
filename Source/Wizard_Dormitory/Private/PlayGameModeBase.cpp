// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameModeBase.h"
#include "NPC.h"
#include "NPCUtils.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void APlayGameModeBase::BeginPlay()
{
	/*
	
	FNPCSeedData TestSeed;
	TestSeed.HairIndex = 1;
	TestSeed.EyeIndex = 0;
	TestSeed.MouthIndex = 2;

	FVector SpawnLocation(0, 0, 100.0f);
	SpawnNPCWithSeed(TestSeed, SpawnLocation);
	
	// �õ� �迭 ����
	GenerateNPCSeeds();
	
	// �׽�Ʈ: ù ��° �õ�� NPC 1�� ����
	if (GeneratedSeeds.Num() > 0)
	{
		FVector SpawnLocation = FVector(0.f, 0.f, 100.f);
		SpawnNPCWithSeed(GeneratedSeeds[0], SpawnLocation);
	}
	*/
	/*
	GenerateNPCSeeds();
	// ������ ��� �õ带 �̿��� NPC ���� �� ����
	const float SpawnSpacing = 200.f; // ���� ����

	for (int32 i = 0; i < GeneratedSeeds.Num(); ++i)
	{
		FVector SpawnLocation = FVector(i * SpawnSpacing, 0.f, 100.f);
		SpawnNPCWithSeed(GeneratedSeeds[i], SpawnLocation);
	}
	*/

	AutoBindReusableNPC();
	// �õ� �迭 ���� �� ù NPC ����
	GenerateNPCSeeds();
	ApplyNextSeed();
	
}

void APlayGameModeBase::GenerateNPCSeeds()
{
	GeneratedSeeds.Empty();

	for (int32 i = 0; i < NumNPCToGenerate; ++i)
	{
		GeneratedSeeds.Add(GenerateRandomSeed());
	}

	UE_LOG(LogTemp, Warning, TEXT("Generated %d NPC Seeds"), GeneratedSeeds.Num());

}
//�׽�Ʈ��
void APlayGameModeBase::SpawnNPCWithSeed(const FNPCSeedData& Seed, const FVector& SpawnLocation)
{
	if (!NPCClass)return;
	FActorSpawnParameters Params;
	ANPC* NPC = GetWorld()->SpawnActor<ANPC>(NPCClass, SpawnLocation, FRotator::ZeroRotator, Params);
	if (NPC)
	{
		FNPCVisualData Visual = ConvertSeedToVisual(Seed, NPCLibrary);
		NPC->ApplyVisual(Visual);
	}
}

FNPCSeedData APlayGameModeBase::GenerateRandomSeed() const
{
	FNPCSeedData Seed;

	if (NPCLibrary.HairMeshes.Num() > 0)
		Seed.HairIndex = FMath::RandRange(0, NPCLibrary.HairMeshes.Num() - 1);

	if (NPCLibrary.EyeUVs.Num() > 0)
		Seed.EyeIndex = FMath::RandRange(0, NPCLibrary.EyeUVs.Num() - 1);

	if (NPCLibrary.MouthUVs.Num() > 0)
		Seed.MouthIndex = FMath::RandRange(0, NPCLibrary.MouthUVs.Num() - 1);

	return Seed;
}



void APlayGameModeBase::EvaluateNPC(bool bAccepted)
{
	// ��� ī��Ʈ
	if (bAccepted)
	{
		++NumAccepted;
	}
	else
	{
		++NumRejected;
	}

	// NPC�� ��� ���� �� ���� �Ǵ� ����
	//ReusableNPC->HandleNPCDecision(bAccepted);
}

void APlayGameModeBase::AutoBindReusableNPC()
{
	if (!ReusableNPC)
	{
		/*
		for (TActorIterator<ANPC> It(GetWorld()); It; ++It)
		{
			ReusableNPC = *It;
			UE_LOG(LogTemp, Warning, TEXT("Auto-bound ReusableNPC: %s"), *ReusableNPC->GetName());
			break;
		}
		*/
		for (TActorIterator<ANPC> It(GetWorld()); It; ++It)
		{
			if (It->ActorHasTag("original"))
			{
				ReusableNPC = *It;
				break;
			}
		}
	}

	if (!ReusableNPC)
	{
		UE_LOG(LogTemp, Error, TEXT("ReusableNPC was not found in level!"));
		return;
	}
}

void APlayGameModeBase::ApplyNextSeed()
{

	if (!GeneratedSeeds.IsValidIndex(CurrentSeedIndex)) return;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("AAAAAAA"));

	if (!ReusableNPC) return;
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("BBBBBB"));

	// �õ� �� ���� �� NPC ����
	const FNPCSeedData& Seed = GeneratedSeeds[CurrentSeedIndex];
	const FNPCVisualData Visual = ConvertSeedToVisual(Seed, NPCLibrary);

	ReusableNPC->ApplyVisual(Visual);
	//ReusableNPC->StartEntrance(); // ���� �ִϸ��̼� ���� (BP)

	++CurrentSeedIndex;
}
