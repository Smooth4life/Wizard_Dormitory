// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NPCSeedData.h"
#include "NPCLibrary.h"
#include "PlayGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class WIZARD_DORMITORY_API APlayGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

    // NPC 블루프린트 클래스
    UPROPERTY(EditDefaultsOnly, Category = "NPC")
    TSubclassOf<class ANPC> NPCClass;

    // NPC 외형 데이터 라이브러리
    UPROPERTY(EditDefaultsOnly, Category = "NPC")
    FNPCLibrary NPCLibrary;

    // 생성할 NPC 수 (블루프린트에서 조절 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    int32 NumNPCToGenerate = 5;

    // 생성된 시드 배열
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    TArray<FNPCSeedData> GeneratedSeeds;

    // 재사용될 NPC (레벨에 배치)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    class ANPC* ReusableNPC;

    // 통과 / 불통과 수 카운트
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    int32 NumAccepted = 0;

    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    int32 NumRejected = 0;

    // 현재 순번 인덱스
    int32 CurrentSeedIndex = 0;

    // 시드 생성
    UFUNCTION(BlueprintCallable, Category = "NPC")
    void GenerateNPCSeeds();

    // 판단 결과에 따라 처리 (통과/불통과)
    UFUNCTION(BlueprintCallable, Category = "NPC")
    void EvaluateNPC(bool bAccepted);

    // 다음 NPC 적용
    void ApplyNextSeed();

    // 테스트용 직접 스폰 (다수 생성용)
    void SpawnNPCWithSeed(const FNPCSeedData& Seed, const FVector& Location);

    // 랜덤 시드 생성
    FNPCSeedData GenerateRandomSeed() const;

};
