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
    //TSubclassOf<ANPC> ReusableNPC;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    class ANPC* GuestNPC;

    // 통과시킨 인원수(정답만)
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    int32 NumAccepted = 0;
    //오답횟수
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    int32 NumRejected = 0;

    // 현재 순번 인덱스
    int32 CurrentSeedIndex = 0;



    // 판단 결과에 따라 처리 (통과/불통과)
    UFUNCTION(BlueprintCallable, Category = "NPC")
    void EvaluateNPC(bool bAccepted);

    // 다음 NPC 적용
    UFUNCTION(BlueprintCallable, Category = "NPC")
    void ApplyNextSeed();


    // 시드 배열 생성 함수
    UFUNCTION(BlueprintCallable, Category = "NPC")
    void GenerateNPCSeeds();

    // 단일 랜덤 시드 생성 함수
    FNPCSeedData GenerateRandomSeed() const;

    FNPCSeedData GenerateGuestSeedFromOriginal(const FNPCSeedData& Original, bool bIsNormal) const;


    void AutoBindReusableNPC();



    // 테스트용 직접 스폰 (다수 생성용)
    void SpawnNPCWithSeed(const FNPCSeedData& Seed, const FVector& Location);
};
