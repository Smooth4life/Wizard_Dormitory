#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NPCNameRow.generated.h"

USTRUCT(BlueprintType)
struct FNPCNameRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;//"Name"은 CSV 열 이름과 일치해야 함
};
