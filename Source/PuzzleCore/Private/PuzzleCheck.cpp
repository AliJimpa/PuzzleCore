// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleCheck.h"
#include "PuzzleDebug.h"
#include "Kismet/KismetSystemLibrary.h"

UWorld *UPuzzleCheck::GetWorld() const
{
    if (MyOwner)
    {
        return MyOwner->GetWorld();
    }
    else
    {
        LOG_ERROR("GetWorld Need PuzzleComponent ad Owner that now is null!");
        return nullptr;
    }
}
void UPuzzleCheck::BeginPuzzle(UPuzzleComponent *Owner)
{
    MyOwner = Owner;
    if (MyOwner == nullptr)
    {
        PRINT("[PuzzleCore]: Please read Log");
        LOG_ERROR("Owner (UPuzzleComponent) is NotValid!");
        return;
    }
    OnBeginPuzzle();
}
bool UPuzzleCheck::ExecuteCheck(UObject *Solver, int order) const
{
    return CheckCondition(Solver, order);
}
void UPuzzleCheck::EndPuzzle(UPuzzleComponent *Owner)
{
    if (Owner == nullptr)
    {
        PRINT("[PuzzleCore]: Please read Log");
        LOG_ERROR("Owner (UPuzzleComponent) is NotValid!");
        return;
    }
    OnEndPuzzle();
    MyOwner = nullptr;
}
void UPuzzleCheck::ResetPuzzle(bool bActive)
{
    OnResetPuzzle(bActive);
}
UPuzzleComponent *UPuzzleCheck::GetOwnerPuzzle() const
{
    return MyOwner;
}
void UPuzzleCheck::PrintDebug(const FString &Message, bool bError)
{
    if (!bShowDebug)
        return;
    if (!MyOwner->GetOwner())
        return;

    FString OwnerName = MyOwner->GetOwner()->GetName(); // Get owner name
    FString ComponentName = GetName();                  // Get self component name
    // Combine strings
    FString FullMessage = FString::Printf(TEXT("%s(%s): %s"), *OwnerName, *ComponentName, *Message);

    FColor TextColor;
    if (MyOwner->GetOwner()->HasAuthority())
    {
        TextColor = bError ? FColor(139, 0, 0) : FColor(204, 204, 0);
    }
    else
    {
        TextColor = bError ? FColor::Red : FColor::Yellow;
    }
    float Duration = bError ? 15.0f : 8.0f; // Select show time
    // Print to screen
    if (GEngine)
    {
        UKismetSystemLibrary::PrintString(GetWorld(), FullMessage, true, false, TextColor, Duration);
    }
    // Optional: also print to log
    if (bError)
    {
        LOG_WARNING("%s", *FullMessage);
    }
    else
    {
        LOG_ERROR("%s", *FullMessage);
    }
}
