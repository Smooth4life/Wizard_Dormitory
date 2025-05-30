// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCSeedData.generated.h"
/**
 * NPC 외형을 결정하기 위한 인덱스 기반 시드 데이터
 * 외형 요소 배열에서 인덱스로 접근할 수 있도록 구성됨
 */

USTRUCT(BlueprintType)
struct FNPCSeedData
{
    GENERATED_BODY()

    //헤어 인덱스
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 HairIndex = 0;

    // 눈 인덱스 (Sprite Sheet UV 목록 기준)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EyeIndex = 0;

    // 입 인덱스 (Sprite Sheet UV 목록 기준)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MouthIndex = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsNormal;

    // 소속을 나타내는 이펙트 인덱스
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AffiliationEffectIndex = 0;

    //이름 인덱스
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 NameIndex = 0;


    // 미래 확장 대비 (예: 피부색, 안경, 모자 등)
    // UPROPERTY(EditAnywhere, BlueprintReadWrite)
    // int32 SkinColorIndex = 0;

};


