// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCSeedData.generated.h"
/**
 * NPC ������ �����ϱ� ���� �ε��� ��� �õ� ������
 * ���� ��� �迭���� �ε����� ������ �� �ֵ��� ������
 */

USTRUCT(BlueprintType)
struct FNPCSeedData
{
    GENERATED_BODY()

    //��� �ε���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HairIndex = 0;

    // �� �ε��� (Sprite Sheet UV ��� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EyeIndex = 0;

    // �� �ε��� (Sprite Sheet UV ��� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MouthIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsNormal;

    // �Ҽ��� ��Ÿ���� ����Ʈ �ε���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AffiliationEffectIndex = 0;

    //�̸� �ε���
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NameIndex = 0;


    // �̷� Ȯ�� ��� (��: �Ǻλ�, �Ȱ�, ���� ��)
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // int32 SkinColorIndex = 0;

};


