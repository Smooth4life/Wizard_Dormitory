// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class WIZARD_DORMITORY_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    // 현재 일차 (게임 진척도)
    UPROPERTY(BlueprintReadWrite, Category = "Stage")
    int32 CurrentDay = 0;

    // 일차별 NPC 수 (블루프린트에서 설정 가능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stage")
    TMap<int32, int32> DayToNPCCount;

    // 현재 일차에 맞는 NPC 수를 반환
    UFUNCTION(BlueprintCallable, Category = "Stage")
    int32 GetNPCCountForCurrentDay() const;

	
};
