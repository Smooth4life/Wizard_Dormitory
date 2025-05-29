// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameModeBase.h"
#include "NPC.h"
#include "NPCUtils.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

void APlayGameModeBase::BeginPlay()
{
	AutoBindReusableNPC();
	// 시드 배열 생성 및 첫 NPC 적용
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
//테스트용
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
		return Original; // 정상인 경우는 원본과 동일
	}

	FNPCSeedData Modified = Original;

	// 최소 1개 이상은 다르게 만들어야 하므로 무작위로 몇 개 변경
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

	// 무작위로 최소 1개 이상 실행
	int32 NumChanges = FMath::RandRange(1, Modifiers.Num());
	Modifiers.Sort([](auto&, auto&) { return FMath::RandBool(); }); // 셔플

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
	// 게스트 NPC에 애니메이션 전달
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
	// 시드 인덱스 확인
	if (!GeneratedSeeds.IsValidIndex(CurrentSeedIndex)) return;

	const FNPCSeedData& OriginalSeed = GeneratedSeeds[CurrentSeedIndex];

	// 오리지널 NPC 적용
	if (ReusableNPC)
	{
		FNPCVisualData Visual = ConvertSeedToVisual(OriginalSeed, NPCLibrary);
		ReusableNPC->ApplyVisual(Visual);
		// ReusableNPC->StartEntrance(); // 필요시 애니메이션 시작
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ReusableNPC is null"));
	}

	// 게스트 NPC 적용
	if (GuestNPC)
	{
		FNPCSeedData GuestSeed = GenerateGuestSeedFromOriginal(OriginalSeed, OriginalSeed.bIsNormal);
		FNPCVisualData GuestVisual = ConvertSeedToVisual(GuestSeed, NPCLibrary);
		GuestNPC->ApplyVisual(GuestVisual);
		// GuestNPC->StartEntrance(); // 필요시 애니메이션 시작
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GuestNPC is null"));
	}

	++CurrentSeedIndex;

	//시드값 적용 확인용
	/*
	UE_LOG(LogTemp, Warning, TEXT("[Original] H=%d E=%d M=%d | IsNormal=%s"),
		OriginalSeed.HairIndex, OriginalSeed.EyeIndex, OriginalSeed.MouthIndex, OriginalSeed.bIsNormal ? TEXT("true") : TEXT("false"));

	FNPCSeedData GuestSeed = GenerateGuestSeedFromOriginal(OriginalSeed, OriginalSeed.bIsNormal);

	UE_LOG(LogTemp, Warning, TEXT("[Guest]    H=%d E=%d M=%d | IsNormal=%s"),
		GuestSeed.HairIndex, GuestSeed.EyeIndex, GuestSeed.MouthIndex, GuestSeed.bIsNormal ? TEXT("true") : TEXT("false"));
		*/

}
