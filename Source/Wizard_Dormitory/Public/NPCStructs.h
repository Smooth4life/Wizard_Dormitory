#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NPCStructs.generated.h"

/**
 * �� ��� ������ (��/�� �ؽ�ó ��ǥ ����)
 */
USTRUCT(BlueprintType)
struct FNPCFaceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D EyeUV;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D MouthUV;
};

/**
 * NPC ��ü ���� ������
 */
USTRUCT(BlueprintType)
struct FNPCVisualData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* HairMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FNPCFaceData FaceData;
};
