// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleCheck.h"
#include "PuzzleDebug.h"

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
