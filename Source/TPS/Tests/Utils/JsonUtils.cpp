// My game copyright

#include "Tests/Utils/JsonUtils.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"

DEFINE_LOG_CATEGORY_STATIC(LogJsonUtils, All, All);

using namespace TPS::Test;

bool JsonUtils::WriteInputData(const FString& FileName, const FInputData& InputData)
{
    TSharedPtr<FJsonObject> MainJsonObject = FJsonObjectConverter::UStructToJsonObject<FInputData>(InputData);
    if (!MainJsonObject.IsValid())
    {
        UE_LOG(LogJsonUtils, Error, TEXT("MainJsonObject isn't valid"));
        return false;
    }

    FString OutputString;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutputString);

    if (!FJsonSerializer::Serialize(MainJsonObject.ToSharedRef(), JsonWriter))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON serialization error"));
        return false;
    };
    if (!JsonWriter->Close())
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JSON writer close error"));
        return false;
    }
    if (!FFileHelper::SaveStringToFile(OutputString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("File saving error [%s]"), *FileName);
        return false;
    }
    return true;
}

bool JsonUtils::ReadInputData(const FString& FileName, FInputData& InputData)
{
    TSharedPtr<FJsonObject> MainJsonObject = MakeShareable(new FJsonObject());

    FString FileContentString;
    if (!FFileHelper::LoadFileToString(FileContentString, *FileName))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("load file error [%s]"), *FileName);
        return false;
    }

    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(FileContentString);
    if (!FJsonSerializer::Deserialize(JsonReader, MainJsonObject))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("Deserialize error"));
        return false;
    }

    if (!FJsonObjectConverter::JsonObjectToUStruct(MainJsonObject.ToSharedRef(), &InputData, 0, 0))
    {
        UE_LOG(LogJsonUtils, Error, TEXT("JsonObjectToUStruct error"));
        return false;
    }

    return true;
}
