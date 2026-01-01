#pragma once

#include "CoreMinimal.h"
#include "PuzzleDebug.h"

DECLARE_LOG_CATEGORY_EXTERN(Log_PuzzleCore, Log, All);

#if !UE_BUILD_SHIPPING
#define LOG(Format, ...) \
    UE_LOG(Log_PuzzleCore, Log, TEXT(Format), ##__VA_ARGS__)
#define LOG_WARNING(Format, ...) \
    UE_LOG(Log_PuzzleCore, Warning, TEXT(Format), ##__VA_ARGS__)
#define LOG_ERROR(Format, ...) \
    UE_LOG(Log_PuzzleCore, Warning, TEXT(Format), ##__VA_ARGS__)
#define PRINT(Format, ...)                                      \
    do                                                           \
    {                                                            \
        if (GEngine)                                             \
        {                                                        \
            GEngine->AddOnScreenDebugMessage(                    \
                -1,                                              \
                5.0f,                                            \
                FColor::Black,                                   \
                FString::Printf(TEXT(Format), ##__VA_ARGS__));   \
        }                                                        \
        else                                                     \
        {                                                        \
            UE_LOG(Log_PuzzleCore, Error, TEXT(Format), ##__VA_ARGS__); \
        }                                                        \
    } while (0)
#else
LOG(Format, ...)
LOG_WARNING(Format, ...)
LOG_ERROR(Format, ...)
PRINT(FString message)
#endif