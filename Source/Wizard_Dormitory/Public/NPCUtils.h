// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCStructs.h"
#include "NPCSeedData.h"
#include "NPCLibrary.h"

//�õ�� ����Ǯ�� ����� NPC�� ���� �����͸� �����ϴ� �Լ�

static FNPCVisualData ConvertSeedToVisual(const FNPCSeedData & Seed, const FNPCLibrary & Library)
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

    return Result;
}
