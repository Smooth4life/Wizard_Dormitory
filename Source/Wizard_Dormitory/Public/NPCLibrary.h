// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCStructs.h"
#include "NiagaraSystem.h"
#include "NPCLibrary.generated.h"

USTRUCT(BlueprintType)
struct FNPCLibrary
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UStaticMesh*> HairMeshes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector2D> EyeUVs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector2D> MouthUVs;

    // 소속 이펙트들
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UNiagaraSystem*> AffiliationEffects;

    // 추가: 소속 이름 리스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AffiliationNames;

};
