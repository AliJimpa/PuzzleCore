// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "PuzzleCheck.generated.h"

class UPuzzleComponent;
/**
 *
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class PUZZLECORE_API UPuzzleCheck : public UObject
{
	GENERATED_BODY()
public:
	virtual UWorld *GetWorld() const override;

public:
	/**
	 * Called when the puzzle begins evaluation for this check.
	 * Use this event to initialize state or cache references needed for condition checking.
	 *
	 * @param Instigator The puzzle component that triggered this check.
	 */
	void BeginPuzzle(UPuzzleComponent *owner);
	/**
	 * Evaluates whether this puzzlecheck condition is satisfied.
	 *
	 * @param Solver The object attempting to solve the puzzle.
	 * @param Order Index or execution order of this check within the puzzle.
	 * @return True if the condition is satisfied.
	 */
	bool ExecuteCheck(UObject *Solver, int order) const;
	/**
	 * Called when the puzzle evaluation ends for this check.
	 * Use this event to clean up state or finalize logic after checking.
	 *
	 * @param Instigator The puzzle component that triggered this check.
	 */
	void EndPuzzle(UPuzzleComponent *owner);
	/**
	 * Called when the puzzle is reset.
	 *
	 * @param bActive Whether the puzzle becomes active after reset.
	 */
	void ResetPuzzle(bool bActive);

protected:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Getter", meta = (BlueprintProtected = "true", ToolTip = "Returns the owner PuzzleComponent"))
	UPuzzleComponent *GetOwnerPuzzle() const;
	UFUNCTION(BlueprintCallable, Category = "PuzzleCheck", meta = (BlueprintProtected = "true", ToolTip = "Prints a string to the screen "))
	void PrintDebug(const FString &Message, bool bError = false);
	UFUNCTION(BlueprintImplementableEvent, Category = "PuzzleCheck", meta = (ToolTip = "Called when the puzzle begins evaluation for this check"))
	void OnBeginPuzzle();
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, Category = "PuzzleCheck", meta = (ToolTip = "Evaluates whether this puzzlecheck condition is satisfied"))
	bool CheckCondition(UObject *Solver, int order) const;
	UFUNCTION(BlueprintImplementableEvent, Category = "PuzzleCheck", meta = (ToolTip = "Called when the puzzle evaluation ends for this check"))
	void OnEndPuzzle();
	UFUNCTION(BlueprintImplementableEvent, Category = "PuzzleCheck", meta = (ToolTip = "Called when the puzzle is reset"))
	void OnResetPuzzle(bool bIsUnavailable);

private:
	UPuzzleComponent *MyOwner = nullptr;
	UPROPERTY(EditAnywhere, Category = "PuzzleCheck", AdvancedDisplay)
	bool bShowDebug = true;
};
