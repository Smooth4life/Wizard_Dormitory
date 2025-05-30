// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCStructs.h"
#include "NPCSeedData.h"
#include "NPCLibrary.h"
#include <NPCNameRow.h>

//�õ�� ����Ǯ�� ����� NPC�� ���� �����͸� �����ϴ� �Լ�

static FNPCVisualData ConvertSeedToVisual(const FNPCSeedData & Seed, const FNPCLibrary & Library, const UDataTable* NameTable)
{
    FNPCVisualData Result;

    // ���
    if (Library.HairMeshes.IsValidIndex(Seed.HairIndex))
    {
        Result.HairMesh = Library.HairMeshes[Seed.HairIndex];
    }

    // ��
    if (Library.EyeUVs.IsValidIndex(Seed.EyeIndex))
    {
        Result.FaceData.EyeUV = Library.EyeUVs[Seed.EyeIndex];
    }

    // ��
    if (Library.MouthUVs.IsValidIndex(Seed.MouthIndex))
    {
        Result.FaceData.MouthUV = Library.MouthUVs[Seed.MouthIndex];
    }
    //���̾ư���Ҽ�
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

    return Result;
}
