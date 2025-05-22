// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCStructs.h"
#include "NPCSeedData.h"
#include "NPCLibrary.h"

//시드와 에셋풀을 사용해 NPC의 외형 데이터를 생성하는 함수

static FNPCVisualData ConvertSeedToVisual(const FNPCSeedData & Seed, const FNPCLibrary & Library)
{
    FNPCVisualData Result;

    // 헤어
    if (Library.HairMeshes.IsValidIndex(Seed.HairIndex))
    {
        Result.HairMesh = Library.HairMeshes[Seed.HairIndex];
    }

    // 눈
    if (Library.EyeUVs.IsValidIndex(Seed.EyeIndex))
    {
        Result.FaceData.EyeUV = Library.EyeUVs[Seed.EyeIndex];
    }

    // 입
    if (Library.MouthUVs.IsValidIndex(Seed.MouthIndex))
    {
        Result.FaceData.MouthUV = Library.MouthUVs[Seed.MouthIndex];
    }

    return Result;
}
