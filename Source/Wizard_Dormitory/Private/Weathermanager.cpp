#include "WeatherManager.h"
#include "HttpModule.h"               // FHttpModule
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Engine/Engine.h"            // GEngine->AddOnScreenDebugMessage

// API Key 정의
const FString AWeatherManager::APIKey = TEXT("18797d2e8a5f940f9d01414856f18736");

AWeatherManager::AWeatherManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AWeatherManager::RequestWeather()
{
    static const TMap<ECityName, FString> CityMap = {
        {ECityName::Seoul,   TEXT("Seoul,kr")},
        {ECityName::Busan,   TEXT("Busan,kr")},
        {ECityName::Incheon, TEXT("Incheon,kr")},
        {ECityName::Daegu,   TEXT("Daegu,kr")},
        {ECityName::Daejeon, TEXT("Daejeon,kr")},
        {ECityName::Ulsan,   TEXT("Ulsan,kr")},
        {ECityName::Jeju,    TEXT("Jeju,kr")},
        {ECityName::Suwon,   TEXT("Suwon,kr")}
    };

    const FString& CityParam = CityMap.Contains(City)
        ? CityMap[City]
        : CityMap[ECityName::Seoul];

    TSharedRef<IHttpRequest> Req = FHttpModule::Get().CreateRequest();
    FString URL = FString::Printf(
        TEXT("https://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s"),
        *CityParam, *APIKey
    );
    Req->SetURL(URL);
    Req->SetVerb(TEXT("GET"));
    Req->OnProcessRequestComplete().BindUObject(this, &AWeatherManager::OnResponseReceived);
    Req->ProcessRequest();
}

void AWeatherManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid() || Response->GetResponseCode() != 200)
    {
        if (GEngine)
        {
            int32 Code = Response.IsValid() ? Response->GetResponseCode() : -1;
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                FString::Printf(TEXT("Weather 요청 실패: %d"), Code));
        }
        return;
    }

    // JSON 파싱
    FString JsonStr = Response->GetContentAsString();
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonStr);
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        const auto& Arr = JsonObject->GetArrayField(TEXT("weather"));
        FString Main = Arr.Num() > 0
            ? Arr[0]->AsObject()->GetStringField(TEXT("main"))
            : FString();

        CurrentCategory = MapWeatherMainToCategory(Main);
        OnWeatherUpdated.Broadcast(CurrentCategory);

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
                FString::Printf(TEXT("날씨: %s → 카테고리 %d"), *Main, (int32)CurrentCategory));
        }
    }
}

EWeatherCategory AWeatherManager::MapWeatherMainToCategory(const FString& WeatherMain) const
{
    if (WeatherMain.Equals(TEXT("Clear"), ESearchCase::IgnoreCase))
        return EWeatherCategory::Clear;
    if (WeatherMain.Equals(TEXT("Rain"), ESearchCase::IgnoreCase) ||
        WeatherMain.Equals(TEXT("Drizzle"), ESearchCase::IgnoreCase) ||
        WeatherMain.Equals(TEXT("Thunderstorm"), ESearchCase::IgnoreCase))
        return EWeatherCategory::Rain;
    if (WeatherMain.Equals(TEXT("Snow"), ESearchCase::IgnoreCase))
        return EWeatherCategory::Snow;

    static const TSet<FString> Clouds = {
        TEXT("Clouds"), TEXT("Mist"), TEXT("Smoke"),
        TEXT("Haze"),   TEXT("Dust"), TEXT("Fog"),
        TEXT("Sand"),   TEXT("Squall")
    };
    if (Clouds.Contains(WeatherMain))
        return EWeatherCategory::Clouds;

    // 네 가지 외 모두 맑음으로
    return EWeatherCategory::Clear;
}

EWeatherCategory AWeatherManager::GetCurrentWeatherCategory() const
{
    return CurrentCategory;
}
