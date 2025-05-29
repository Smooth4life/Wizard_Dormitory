// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameModeBase.h"
#include "NPC.h"
#include "NPCUtils.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void APlayGameModeBase::BeginPlay()
{
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

	Seed.bIsNormal = FMath::RandBool();

	return Seed;
}

FNPCSeedData APlayGameModeBase::GenerateGuestSeedFromOriginal(const FNPCSeedData& Original, bool bIsNormal) const
{
	if (bIsNormal)
	{
		return Original; // ������ ���� ������ ����
	}

	FNPCSeedData Modified = Original;

	// �ּ� 1�� �̻��� �ٸ��� ������ �ϹǷ� �������� �� �� ����
	TArray<TFunction<void()>> Modifiers;

	if (NPCLibrary.HairMeshes.Num() > 1)
	{
		Modifiers.Add([&]() {
			do {
				Modified.HairIndex = FMath::RandRange(0, NPCLibrary.HairMeshes.Num() - 1);
			} while (Modified.HairIndex == Original.HairIndex);
			});
	}

	if (NPCLibrary.EyeUVs.Num() > 1)
	{
		Modifiers.Add([&]() {
			do {
				Modified.EyeIndex = FMath::RandRange(0, NPCLibrary.EyeUVs.Num() - 1);
			} while (Modified.EyeIndex == Original.EyeIndex);
			});
	}

	if (NPCLibrary.MouthUVs.Num() > 1)
	{
		Modifiers.Add([&]() {
			do {
				Modified.MouthIndex = FMath::RandRange(0, NPCLibrary.MouthUVs.Num() - 1);
			} while (Modified.MouthIndex == Original.MouthIndex);
			});

	}

	// �������� �ּ� 1�� �̻� ����
	int32 NumChanges = FMath::RandRange(1, Modifiers.Num());
	Modifiers.Sort([](auto&, auto&) { return FMath::RandBool(); }); // ����

	for (int32 i = 0; i < NumChanges; ++i)
	{
		Modifiers[i]();
	}

	Modified.bIsNormal = false;
	return Modified;
}





void APlayGameModeBase::EvaluateNPC(bool bAccepted)
{
	if (!GeneratedSeeds.IsValidIndex(CurrentSeedIndex - 1))
	{
		UE_LOG(LogTemp, Warning, TEXT("No current seed to evaluate"));
		return;
	}

	const FNPCSeedData& CurrentSeed = GeneratedSeeds[CurrentSeedIndex - 1];

	const bool bIsCorrect = (bAccepted == CurrentSeed.bIsNormal);

	if (bIsCorrect)
	{
		++NumAccepted;
		UE_LOG(LogTemp, Warning, TEXT("true"));
	}
	else
	{
		++NumRejected;
		UE_LOG(LogTemp, Warning, TEXT("false"));
	}
	/*
	// �Խ�Ʈ NPC�� �ִϸ��̼� ����
	if (GuestNPC)
	{
		GuestNPC->HandleNPCDecision(bAccepted);
	}
	*/
}

void APlayGameModeBase::AutoBindReusableNPC()
{
	if (!ReusableNPC)
	{
		for (TActorIterator<ANPC> It(GetWorld()); It; ++It)
		{
			if (!ReusableNPC && It->ActorHasTag("original"))
			{
				ReusableNPC = *It;
			}
			else if (!GuestNPC && It->ActorHasTag("guest"))
			{
				GuestNPC = *It;
			}

			if (ReusableNPC && GuestNPC) break;
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
	// �õ� �ε��� Ȯ��
	if (!GeneratedSeeds.IsValidIndex(CurrentSeedIndex)) return;

	const FNPCSeedData& OriginalSeed = GeneratedSeeds[CurrentSeedIndex];

	// �������� NPC ����
	if (ReusableNPC)
	{
		FNPCVisualData Visual = ConvertSeedToVisual(OriginalSeed, NPCLibrary);
		ReusableNPC->ApplyVisual(Visual);
		// ReusableNPC->StartEntrance(); // �ʿ�� �ִϸ��̼� ����
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ReusableNPC is null"));
	}

	// �Խ�Ʈ NPC ����
	if (GuestNPC)
	{
		FNPCSeedData GuestSeed = GenerateGuestSeedFromOriginal(OriginalSeed, OriginalSeed.bIsNormal);
		FNPCVisualData GuestVisual = ConvertSeedToVisual(GuestSeed, NPCLibrary);
		GuestNPC->ApplyVisual(GuestVisual);
		// GuestNPC->StartEntrance(); // �ʿ�� �ִϸ��̼� ����
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GuestNPC is null"));
	}

	++CurrentSeedIndex;

	//�õ尪 ���� Ȯ�ο�
	/*
	UE_LOG(LogTemp, Warning, TEXT("[Original] H=%d E=%d M=%d | IsNormal=%s"),
		OriginalSeed.HairIndex, OriginalSeed.EyeIndex, OriginalSeed.MouthIndex, OriginalSeed.bIsNormal ? TEXT("true") : TEXT("false"));

	FNPCSeedData GuestSeed = GenerateGuestSeedFromOriginal(OriginalSeed, OriginalSeed.bIsNormal);

	UE_LOG(LogTemp, Warning, TEXT("[Guest]    H=%d E=%d M=%d | IsNormal=%s"),
		GuestSeed.HairIndex, GuestSeed.EyeIndex, GuestSeed.MouthIndex, GuestSeed.bIsNormal ? TEXT("true") : TEXT("false"));
		*/

}
