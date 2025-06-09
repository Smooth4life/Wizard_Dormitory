#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Sound/SoundBase.h"
#include "NPCDialogueRow.generated.h"

USTRUCT(BlueprintType)
struct FNPCDialogueRow : public FTableRowBase
{
    GENERATED_BODY()

    // �ڸ� �ؽ�Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Subtitle;

    // �����Ǵ� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USoundBase* Sound;

    // ���� �б�� �±� (������ ������)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName StyleTag;
};
