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

    // 추가: 학번 (시드 기반 생성된 값)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 StudentID;

    // 추가: 소속명 (예: "마법학과", "기술과")
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AffiliationName;

};
