// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

int32 UMyGameInstance::GetNPCCountForCurrentDay() const
{
    const int32* Found = DayToNPCCount.Find(CurrentDay);
    return Found ? *Found : 0;
}
