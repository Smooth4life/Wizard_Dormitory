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

    // 에셋 풀 (블루프린트에서 설정 가능)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
    FNPCLibrary NPCLibrary;

    // NPC 생성 함수
    void SpawnNPCWithSeed(const FNPCSeedData& Seed, const FVector& SpawnLocation);



};
