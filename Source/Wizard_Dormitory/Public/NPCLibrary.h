// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCStructs.h"
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
};
