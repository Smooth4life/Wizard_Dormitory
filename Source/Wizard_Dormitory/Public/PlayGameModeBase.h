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

    // NPC �������Ʈ Ŭ����
    UPROPERTY(EditDefaultsOnly, Category = "NPC")
    TSubclassOf<class ANPC> NPCClass;

    // ���� Ǯ (�������Ʈ���� ���� ����)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "NPC")
    FNPCLibrary NPCLibrary;

    // NPC ���� �Լ�
    void SpawnNPCWithSeed(const FNPCSeedData& Seed, const FVector& SpawnLocation);



};
