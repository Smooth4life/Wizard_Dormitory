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

    // NPC ���� ������ ���̺귯��
    UPROPERTY(EditDefaultsOnly, Category = "NPC")
    FNPCLibrary NPCLibrary;

    // ������ NPC �� (�������Ʈ���� ���� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    int32 NumNPCToGenerate = 5;

    // ������ �õ� �迭
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    TArray<FNPCSeedData> GeneratedSeeds;

    // ����� NPC (������ ��ġ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    class ANPC* ReusableNPC;
    //TSubclassOf<ANPC> ReusableNPC;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
    class ANPC* GuestNPC;

    // �����Ų �ο���(���丸)
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    int32 NumAccepted = 0;
    //����Ƚ��
    UPROPERTY(BlueprintReadOnly, Category = "NPC")
    int32 NumRejected = 0;

    // ���� ���� �ε���
    int32 CurrentSeedIndex = 0;



    // �Ǵ� ����� ���� ó�� (���/�����)
    UFUNCTION(BlueprintCallable, Category = "NPC")
    void EvaluateNPC(bool bAccepted);

    // ���� NPC ����
    UFUNCTION(BlueprintCallable, Category = "NPC")
    void ApplyNextSeed();


    // �õ� �迭 ���� �Լ�
    UFUNCTION(BlueprintCallable, Category = "NPC")
    void GenerateNPCSeeds();

    // ���� ���� �õ� ���� �Լ�
    FNPCSeedData GenerateRandomSeed() const;

    FNPCSeedData GenerateGuestSeedFromOriginal(const FNPCSeedData& Original, bool bIsNormal) const;


    void AutoBindReusableNPC();



    // �׽�Ʈ�� ���� ���� (�ټ� ������)
    void SpawnNPCWithSeed(const FNPCSeedData& Seed, const FVector& Location);
};
