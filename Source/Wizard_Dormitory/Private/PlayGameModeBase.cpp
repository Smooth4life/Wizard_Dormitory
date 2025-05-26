// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameModeBase.h"
#include "NPC.h"
#include "NPCUtils.h"
#include "Kismet/GameplayStatics.h"

void APlayGameModeBase::BeginPlay()
{
	/*
	
	FNPCSeedData TestSeed;
	TestSeed.HairIndex = 1;
	TestSeed.EyeIndex = 0;
	TestSeed.MouthIndex = 2;

	FVector SpawnLocation(0, 0, 100.0f);
	SpawnNPCWithSeed(TestSeed, SpawnLocation);
	
	// 시드 배열 생성
	GenerateNPCSeeds();
	
	// 테스트: 첫 번째 시드로 NPC 1명 생성
	if (GeneratedSeeds.Num() > 0)
	{
		FVector SpawnLocation = FVector(0.f, 0.f, 100.f);
		SpawnNPCWithSeed(GeneratedSeeds[0], SpawnLocation);
	}
	
	// 생성된 모든 시드를 이용해 NPC 여러 명 스폰
	const float SpawnSpacing = 200.f; // 간격 설정

	for (int32 i = 0; i < GeneratedSeeds.Num(); ++i)
	{
		FVector SpawnLocation = FVector(i * SpawnSpacing, 0.f, 100.f);
		SpawnNPCWithSeed(GeneratedSeeds[i], SpawnLocation);
	}
	*/
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
	// 통계 카운트
	if (bAccepted)
	{
		++NumAccepted;
	}
	else
	{
		++NumRejected;
	}

	// NPC에 결과 전달 → 퇴장 또는 복귀
	ReusableNPC->HandleNPCDecision(bAccepted);
}


void APlayGameModeBase::ApplyNextSeed()
{
	if (!GeneratedSeeds.IsValidIndex(CurrentSeedIndex)) return;
	if (!ReusableNPC) return;

	// 시드 → 외형 → NPC 적용
	const FNPCSeedData& Seed = GeneratedSeeds[CurrentSeedIndex];
	const FNPCVisualData Visual = ConvertSeedToVisual(Seed, NPCLibrary);

	ReusableNPC->ApplyVisual(Visual);
	ReusableNPC->StartEntrance(); // 입장 애니메이션 시작 (BP)

	++CurrentSeedIndex;
}
