// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCStructs.h"
#include "NPCSeedData.h"
#include "NPCLibrary.h"
#include <NPCNameRow.h>

//시드와 에셋풀을 사용해 NPC의 외형 데이터를 생성하는 함수

static FNPCVisualData ConvertSeedToVisual(const FNPCSeedData & Seed, const FNPCLibrary & Library, const UDataTable* NameTable)
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
    //나이아가라소속
    if (Library.AffiliationEffects.IsValidIndex(Seed.AffiliationEffectIndex))
    {
        Result.AffiliationEffect = Library.AffiliationEffects[Seed.AffiliationEffectIndex];
    }

    if (NameTable)
    {
        TArray<FName> RowNames = NameTable->GetRowNames();
        if (RowNames.IsValidIndex(Seed.NameIndex))
        {
            const FNPCNameRow* NameRow = NameTable->FindRow<FNPCNameRow>(RowNames[Seed.NameIndex], TEXT("GetName"));
            if (NameRow)
            {
                Result.DisplayName = NameRow->Name;
            }
        }
    }

    // StudentID와 AffiliationName 세팅
    Result.StudentID = Seed.StudentID;

    if (Library.AffiliationNames.IsValidIndex(Seed.AffiliationEffectIndex))
    {
        Result.AffiliationName = Library.AffiliationNames[Seed.AffiliationEffectIndex];
    }
    else
    {
        Result.AffiliationName = TEXT("미지정");
    }
   

    return Result;
}
