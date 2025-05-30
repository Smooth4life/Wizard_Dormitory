#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NPCNameRow.generated.h"

USTRUCT(BlueprintType)
struct FNPCNameRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;//"Name"�� CSV �� �̸��� ��ġ�ؾ� ��
};
