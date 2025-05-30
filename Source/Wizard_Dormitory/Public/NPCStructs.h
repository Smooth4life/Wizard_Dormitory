#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NiagaraSystem.h"
#include "NPCStructs.generated.h"
/**
 * 얼굴 요소 데이터 (눈/입 텍스처 좌표 정보)
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
 * NPC 전체 외형 데이터
 */
USTRUCT(BlueprintType)
struct FNPCVisualData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* HairMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FNPCFaceData FaceData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UNiagaraSystem* AffiliationEffect = nullptr;

    // 실제로 사용할 NPC 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DisplayName;

};
