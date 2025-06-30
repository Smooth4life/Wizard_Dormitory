// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameModeBase.h"
#include "NPC.h"
#include "NPCUtils.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "MyGameInstance.h"

void APlayGameModeBase::BeginPlay()
{
	/*//StartGame으로 이동
	SetupUVPresets(); // UV 자동 생성
	AutoBindReusableNPC();
	// 시드 배열 생성 및 첫 NPC 적용
	GenerateNPCSeeds();
	ApplyNextSeed();
	*/
	const UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (!GI) return;

	int32 NPCCount = GI->GetNPCCountForCurrentDay();

	if (NPCCount <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Day %d의 NPC 수가 0이라 게임 시작 생략"), GI->CurrentDay);
		return;
	}

	NumNPCToGenerate = NPCCount;

	UE_LOG(LogTemp, Log, TEXT("[%d일차] NPC %d명 생성 시작"), GI->CurrentDay, NPCCount);

	StartGame();
}

void APlayGameModeBase::GenerateNPCSeeds()
{
	GeneratedSeeds.Empty();

	int32 Created = 0;
	int32 Attempts = 0;
	const int32 MaxAttempts = NumNPCToGenerate * 10; // 무한 루프 방지용

	while (Created < NumNPCToGenerate && Attempts < MaxAttempts)
	{
		++Attempts;

		FNPCSeedData NewSeed = GenerateRandomSeed();

		// 중복 검사
		bool bIsDuplicate = GeneratedSeeds.ContainsByPredicate([&](const FNPCSeedData& Existing)
			{
				return NewSeed.HairIndex == Existing.HairIndex &&
					NewSeed.EyeIndex == Existing.EyeIndex &&
					NewSeed.MouthIndex == Existing.MouthIndex &&
					NewSeed.bIsNormal == Existing.bIsNormal;
			});

		if (!bIsDuplicate)
		{
			GeneratedSeeds.Add(NewSeed);
			++Created;
		}
	}

	if (Created < NumNPCToGenerate)
	{
		UE_LOG(LogTemp, Warning, TEXT("중복 회피 실패: %d개만 생성됨 (요청: %d)"), Created, NumNPCToGenerate);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" %d개의 중복 없는 시드 생성 완료"), Created);
	}


}
//테스트용
void APlayGameModeBase::SpawnNPCWithSeed(const FNPCSeedData& Seed, const FVector& SpawnLocation)
{
	if (!NPCClass)return;
	FActorSpawnParameters Params;
	ANPC* NPC = GetWorld()->SpawnActor<ANPC>(NPCClass, SpawnLocation, FRotator::ZeroRotator, Params);
	/*
	if (NPC)
	{
		FNPCVisualData Visual = ConvertSeedToVisual(Seed, NPCLibrary);
		NPC->ApplyVisual(Visual);
	}
	*/
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

	if (NPCLibrary.AffiliationEffects.Num() > 0)
		Seed.AffiliationEffectIndex = FMath::RandRange(0, NPCLibrary.AffiliationEffects.Num() - 1);

	if (NameTable)
	{
		TArray<FName> RowNames = NameTable->GetRowNames();
		if (RowNames.Num() > 0)
		{
			Seed.NameIndex = FMath::RandRange(0, RowNames.Num() - 1);
		}
	}

	Seed.StudentID = GenerateStudentID(Seed.AffiliationEffectIndex);

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

	// 머리 인덱스 변경 가능할 경우만 Modifier 추가
	if (NPCLibrary.HairMeshes.Num() > 1)// 2개 이상 있어야 다른 값 선택 가능
	{
		// 람다 함수: HairIndex를 Original과 다른 값으로 무작위 설정
		// 이 함수는 지금 실행하지 않고, 나중에 랜덤으로 선택하여 실행함
		Modifiers.Add([&]() {
			do {
				Modified.HairIndex = FMath::RandRange(0, NPCLibrary.HairMeshes.Num() - 1);
			} while (Modified.HairIndex == Original.HairIndex);
			});
	}

	if (NPCLibrary.EyeUVs.Num() > 1)
	{
		// EyeIndex를 무작위로 바꾸되, 원본과 같은 값은 피함
		Modifiers.Add([&]() {
			do {
				Modified.EyeIndex = FMath::RandRange(0, NPCLibrary.EyeUVs.Num() - 1);
			} while (Modified.EyeIndex == Original.EyeIndex);//Modified.EyeIndex에 랜덤한 값을 넣되,그 값이 Original.EyeIndex와 같다면 다시 뽑아라
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
	// 나이아가라 이펙트 인덱스 변경(소속)
	if (NPCLibrary.AffiliationEffects.Num() > 1)
	{
		Modifiers.Add([&]() {
			do {
				Modified.AffiliationEffectIndex = FMath::RandRange(0, NPCLibrary.AffiliationEffects.Num() - 1);
			} while (Modified.AffiliationEffectIndex == Original.AffiliationEffectIndex);
			});
	}

	// 이름 인덱스 변경
	if (NameTable)
	{
		TArray<FName> RowNames = NameTable->GetRowNames();
		if (RowNames.Num() > 1)
		{
			Modifiers.Add([&, RowNames]() {
				do {
					Modified.NameIndex = FMath::RandRange(0, RowNames.Num() - 1);
				} while (Modified.NameIndex == Original.NameIndex);
				});
		}
	}

	// 무작위로 몇 개 항목을 변경할지 결정 (최소 1개 ~ 최대 전체)
	int32 NumChanges = FMath::RandRange(1, Modifiers.Num());

	// Modifiers 배열의 순서를 무작위로 섞음 (셔플)
	// 이 정렬 방식은 두 요소의 순서를 비교할 때 FMath::RandBool()을 사용하여 무작위 결과를 만들기 때문에
	// 결과적으로 전체 배열의 순서가 랜덤하게 섞이게 됨
	Modifiers.Sort([](auto&, auto&) { return FMath::RandBool(); }); // 셔플

	// 앞에서부터 NumChanges 개수만큼의 람다를 실행하여 실제로 Modified 값을 변경
	for (int32 i = 0; i < NumChanges; ++i)
	{
		Modifiers[i]();// 여기서 HairIndex, EyeIndex, MouthIndex 중 일부가 바뀜
	}

	Modified.StudentID = GenerateStudentID(Modified.AffiliationEffectIndex);
	Modified.bIsNormal = false;
	return Modified;
}


int64 APlayGameModeBase::GenerateStudentID(int32 AffiliationIndex) const
{
	const int32 Year = FDateTime::Now().GetYear(); // ex. 2025
	const int32 RandomPart = FMath::RandRange(0, 9999); // 0000~9999

	// 학번 구조: YYYYASSSS (예: 202530042)
	// A: 소속, SSSS: 랜덤값
	return Year * 100000 + AffiliationIndex * 10000 + RandomPart;
}



void APlayGameModeBase::EvaluateNPC(bool bAccepted)
{
	if (!GeneratedSeeds.IsValidIndex(CurrentSeedIndex - 1))
	{
		UE_LOG(LogTemp, Warning, TEXT("No current seed to evaluate"));
		return;
	}

	const FNPCSeedData& CurrentSeed = GeneratedSeeds[CurrentSeedIndex - 1];
	
	/////////////////////////
	const bool bIsActuallyNormal = CurrentSeed.bIsNormal;

	// 판단 분기
	if (bAccepted && bIsActuallyNormal)
	{
		++NumAccepted;
		UE_LOG(LogTemp, Warning, TEXT("정상 NPC를 통과시켰습니다. (정답)"));
	}
	else if (bAccepted && !bIsActuallyNormal)
	{
		++NumFake;
		UE_LOG(LogTemp, Warning, TEXT("비정상 NPC를 통과시켰습니다! (오답)"));
	}
	else if (!bAccepted && bIsActuallyNormal)
	{
		++NumRejected;
		UE_LOG(LogTemp, Warning, TEXT("정상 NPC를 불통과시켰습니다! (오답)"));
	}

	if ((NumFake + NumRejected) > NumNPCToGenerate * 0.5f)
	{
		UGameplayStatics::OpenLevel(this, FName("M_GameOver"));

	}

	if (NumNPCToGenerate == CurrentSeedIndex)
	{
		bGameEnd = true;
		EndGame();
	}

	/*
	else if (!bAccepted && !bIsActuallyNormal)
	{
		++NumAccepted;
		UE_LOG(LogTemp, Warning, TEXT("비정상 NPC를 불통과시켰습니다. (정답)"));
	}
	*/

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

void APlayGameModeBase::SetupUVPresets()
{
	NPCLibrary.EyeUVs = GenerateVerticalUVs(NumEyeFrames);
	NPCLibrary.MouthUVs = GenerateVerticalUVs(NumMouthFrames);

	UE_LOG(LogTemp, Warning, TEXT("EyeUVs = %d개, MouthUVs = %d개 자동 생성됨"), NumEyeFrames, NumMouthFrames);
}

void APlayGameModeBase::StartGame()
{
	SetupUVPresets();
	AutoBindReusableNPC();
	GenerateNPCSeeds();
	ApplyNextSeed();
}

void APlayGameModeBase::ApplyNextSeed()
{
	// 시드 인덱스 확인
	if (!GeneratedSeeds.IsValidIndex(CurrentSeedIndex)) return;

	const FNPCSeedData& OriginalSeed = GeneratedSeeds[CurrentSeedIndex];

	// 오리지널 NPC 적용
	if (ReusableNPC)
	{
		FNPCVisualData Visual = ConvertSeedToVisual(OriginalSeed, NPCLibrary, NameTable);
		ReusableNPC->ApplyVisual(Visual);
		// ReusableNPC->StartEntrance(); // 필요시 애니메이션 시작
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ReusableNPC is null"));
	}

	FNPCSeedData GuestSeed = GenerateGuestSeedFromOriginal(OriginalSeed, OriginalSeed.bIsNormal);
	
	// 게스트 NPC 적용
	if (GuestNPC)
	{
		//FNPCSeedData GuestSeed = GenerateGuestSeedFromOriginal(OriginalSeed, OriginalSeed.bIsNormal);
		FNPCVisualData GuestVisual = ConvertSeedToVisual(GuestSeed, NPCLibrary, NameTable);
		GuestNPC->ApplyVisual(GuestVisual);
		// GuestNPC->StartEntrance(); // 필요시 애니메이션 시작
		// 필요 시 즉시 이펙트 표시 (테스트용 또는 자동 표시용)
		// GuestNPC->ShowAffiliationEffect();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GuestNPC is null"));
	}

	++CurrentSeedIndex;

	
	// 시드값 적용 확인용
	//const FNPCSeedData GuestSeed = GenerateGuestSeedFromOriginal(OriginalSeed, OriginalSeed.bIsNormal);

	if (NameTable)
	{
		TArray<FName> RowNames = NameTable->GetRowNames();

		const FNPCNameRow* OriginalNameRow = RowNames.IsValidIndex(OriginalSeed.NameIndex)
			? NameTable->FindRow<FNPCNameRow>(RowNames[OriginalSeed.NameIndex], TEXT("LogOriginal"))
			: nullptr;

		const FNPCNameRow* GuestNameRow = RowNames.IsValidIndex(GuestSeed.NameIndex)
			? NameTable->FindRow<FNPCNameRow>(RowNames[GuestSeed.NameIndex], TEXT("LogGuest"))
			: nullptr;

		UE_LOG(LogTemp, Warning, TEXT("[Original] H=%d E=%d M=%d A=%d | NameIndex=%d Name=%s | ID=%lld | IsNormal=%s"),
			OriginalSeed.HairIndex,
			OriginalSeed.EyeIndex,
			OriginalSeed.MouthIndex,
			OriginalSeed.AffiliationEffectIndex,
			OriginalSeed.NameIndex,
			OriginalNameRow ? *OriginalNameRow->Name : TEXT("NULL"),
			OriginalSeed.StudentID,
			OriginalSeed.bIsNormal ? TEXT("true") : TEXT("false"));

		UE_LOG(LogTemp, Warning, TEXT("[Guest]    H=%d E=%d M=%d A=%d | NameIndex=%d Name=%s | ID=%lld | IsNormal=%s"),
			GuestSeed.HairIndex,
			GuestSeed.EyeIndex,
			GuestSeed.MouthIndex,
			GuestSeed.AffiliationEffectIndex,
			GuestSeed.NameIndex,
			GuestNameRow ? *GuestNameRow->Name : TEXT("NULL"),
			GuestSeed.StudentID,
			GuestSeed.bIsNormal ? TEXT("true") : TEXT("false"));
	}

}
