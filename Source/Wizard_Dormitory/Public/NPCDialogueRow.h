#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "NPCDialogueRow.generated.h"

USTRUCT(BlueprintType)
struct FNPCDialogueRow : public FTableRowBase
{
    GENERATED_BODY()

    // 자막 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Subtitle;

    // 대응되는 사운드
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundBase* Sound;

    // 선택 분기용 태그 (지금은 사용안함)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName StyleTag;
};
