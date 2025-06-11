#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"                     // FHttpRequestPtr, FHttpResponsePtr
#include "WeatherManager.generated.h"

// 도시 이름
UENUM(BlueprintType)
enum class ECityName : uint8
{
    Seoul   UMETA(DisplayName = "서울"),
    Busan   UMETA(DisplayName = "부산"),
    Incheon UMETA(DisplayName = "인천"),
    Daegu   UMETA(DisplayName = "대구"),
    Daejeon UMETA(DisplayName = "대전"),
    Ulsan   UMETA(DisplayName = "울산"),
    Jeju    UMETA(DisplayName = "제주"),
    Suwon   UMETA(DisplayName = "경기도")  // 내부 Suwon, UI 표시 “경기도”
};

// 날씨 카테고리
UENUM(BlueprintType)
enum class EWeatherCategory : uint8
{
    Clear  UMETA(DisplayName = "맑음"),
    Rain   UMETA(DisplayName = "비"),
    Snow   UMETA(DisplayName = "눈"),
    Clouds UMETA(DisplayName = "흐림")
};

// 날씨 업데이트 이벤트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeatherUpdated, EWeatherCategory, Category);

UCLASS()
class WIZARD_DORMITORY_API AWeatherManager : public AActor
{
    GENERATED_BODY()

public:
    AWeatherManager();

    // 위젯에서 선택한 도시
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weather")
    ECityName City;

    // API 호출 트리거 (블루프린트 호출 가능)
    UFUNCTION(BlueprintCallable, Category = "Weather")
    void RequestWeather();

    // 마지막 받아온 카테고리
    UFUNCTION(BlueprintCallable, Category = "Weather")
    EWeatherCategory GetCurrentWeatherCategory() const;

    // 결과를 블루프린트에 브로드캐스트
    UPROPERTY(BlueprintAssignable, Category = "Weather")
    FOnWeatherUpdated OnWeatherUpdated;

private:
    // 현재 상태 저장 (기본: 맑음)
    EWeatherCategory CurrentCategory = EWeatherCategory::Clear;

    // 하드코딩된 API Key
    static const FString APIKey;

    // HTTP 응답 콜백
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    // weather.main → 카테고리 매핑
    EWeatherCategory MapWeatherMainToCategory(const FString& WeatherMain) const;
};
