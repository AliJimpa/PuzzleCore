// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"

class UPuzzleCheck;

UENUM(BlueprintType)
enum class EPuzzleState : uint8
{
	Unavailable UMETA(DisplayName = "Unavailable"),
	Locked UMETA(DisplayName = "Locked"),
	Solved UMETA(DisplayName = "Solved"),
	Failed UMETA(DisplayName = "Failed")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStateChanged, EPuzzleState, NewState, UPuzzleComponent *, Puzzle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSolved, UPuzzleComponent *, Puzzle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFailed, UPuzzleComponent *, Puzzle);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReset, bool, bActive, UPuzzleComponent *, Puzzle);

UCLASS(Blueprintable, ClassGroup = (PuzzleCore), meta = (BlueprintSpawnableComponent = false, BlueprintType = true))
class PUZZLECORE_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPuzzleComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	/** Automatically makes the puzzle available when the game starts */
	UPROPERTY(EditAnywhere, Category = "Puzzle", meta = (ToolTip = "Automatically makes the puzzle available when the game starts"))
	bool bAutoAvailable = true;
	/** Display name of the puzzle */
	UPROPERTY(EditAnywhere, Category = "Puzzle", meta = (ToolTip = "Unique name used to identify this puzzle"))
	FName PuzzleName;
	/** List of requirements that must be satisfied to solve the puzzle */
	UPROPERTY(EditAnywhere, Instanced, Category = "Puzzle", meta = (ToolTip = "Conditions that must be met before the puzzle can be solved"))
	TArray<UPuzzleCheck *> Requirements;
	/** Automatically resets the puzzle after a failed attempt */
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Puzzle", meta = (ToolTip = "If enabled, the puzzle will automatically reset after a failed puzzle"))
	bool bAutoReset = false;
	/** Automatically resets the puzzle after a failed attempt */
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Puzzle", meta = (ToolTip = "If enabled, the puzzle can be attempted again even after it has failed. When disabled, once the puzzle enters the Failed state, further attempts are blocked & sould reset Puzzle"))
	bool bAllowAttemptAfterFailure = true;
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Puzzle", meta = (ToolTip = "When enabled, only a minimum number of requirements must be satisfied. When disabled, all requirements are required."))
	bool bUseMinimumRequirement = false;
	/** Minimum number of requirements that must be satisfied */
	UPROPERTY(EditAnywhere, AdvancedDisplay, Category = "Puzzle", meta = (EditCondition = "bUseMinimumRequirement", EditConditionHides, ToolTip = "Minimum number of requirements that must be satisfied to solve the puzzle, 0 means all condition should check"))
	int32 MinimumRequirement = 0;
	/** Current runtime state of the puzzle */
	EPuzzleState PuzzleState = EPuzzleState::Unavailable;
	/** Number of solve attempts */
	int32 TryCount;
	/** Chache the order of puzzlecheck when tray to solve */
	int OrderCache = 0;
	/**
	 * Changes the current puzzle state and triggers state-related events.
	 * @param NewState The new state to apply.
	 */
	void
	SetState(const EPuzzleState NewState);

protected:
	/**
	 * Called before the puzzle's internal requirement validation.
	 * Override this function to add custom or game-specific checks.
	 * Returning false will prevent the puzzle from being solved.
	 *
	 * @param PuzzleCheck Rule object to evaluate.
	 * @return True if the rule is satisfied.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Puzzle", meta = (ToolTip = "Called before the puzzle's internal requirement check. Override to add custom validation logic.Returning false prevents the call to 'Check Condition' in 'PuzzleCheck'."))
	bool PrePuzzleCheck(UPuzzleCheck *PuzlleCheck) const;

public:
	/** Returns the current puzzle state */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Getter", meta = (ToolTip = "Returns the current state of the puzzle"))
	EPuzzleState GetPuzzleState() const;
	/** Returns true if the puzzle has been solved */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Getter", meta = (ToolTip = "Returns true if the puzzle is solved"))
	bool IsPuzzleSolved() const;
	/** Returns true if the puzzle is currently locked */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Getter", meta = (ToolTip = "Returns true if the puzzle is locked"))
	bool IsPuzzleLocked() const;
	/** Returns true if the puzzle is active */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Getter", meta = (ToolTip = "Returns true if the puzzle isNot Unavailable"))
	bool IsPuzzleActive() const;
	/** Returns the puzzle name */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Getter", meta = (ToolTip = "Returns the puzzle name"))
	FName GetPuzzleName() const;
	/** Returns the number of solve attempts */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle|Getter", meta = (ToolTip = "Returns the number of times the puzzle was attempted"))
	int32 GetTryCount() const;
	/**
	 * Determines whether the specified solver can solve this puzzle.
	 *
	 * If minimum requirement mode is disabled, all puzzle checks must pass.
	 * If enabled, the puzzle is considered solved once the minimum number of checks succeed.
	 *
	 * @param Solver The object attempting to solve the puzzle.
	 * @return True if the puzzle can be solved; otherwise false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Puzzle", meta = (ToolTip = "Returns true if the specified solver meets the conditions to solve this puzzle"))
	bool CanSolvePuzzle(UObject *Solver) const;
	/**
	 * Attempts to solve the puzzle.
	 *
	 * @param Solver Object attempting to solve the puzzle.
	 * @return True if the puzzle solved.
	 */
	UFUNCTION(BlueprintCallable, Category = "Puzzle", meta = (ToolTip = "Attempts to solve the puzzle and returns 'True' if the puzzle solved"))
	bool TrySolvePuzzle(UObject *Solver);
	/**
	 * Resets the puzzle.
	 *
	 * @param bActive If true, the puzzle becomes active after reset.
	 */
	UFUNCTION(BlueprintCallable, Category = "Puzzle", meta = (ToolTip = "Evaluates Reset event in all requirement and then if active true set state to locked and if false set state to Unavailable"))
	void ResetPuzzle(bool bActive = true);
	/**
	 * Enables or disables the puzzle availability.
	 *
	 * @param bEnable Whether the puzzle is available.
	 */
	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	void SetAvaliblePuzzle(bool bEnable = true);
	/** Called when the puzzle state changes */
	UPROPERTY(BlueprintAssignable, Category = "Puzzle|Event")
	FOnStateChanged OnStateChanged;
	/** Called when the puzzle is solved */
	UPROPERTY(BlueprintAssignable, Category = "Puzzle|Event")
	FOnSolved OnSolved;
	/** Called when the puzzle fails */
	UPROPERTY(BlueprintAssignable, Category = "Puzzle|Event")
	FOnFailed OnFailed;
	/** Called when the puzzle is reset */
	UPROPERTY(BlueprintAssignable, Category = "Puzzle|Event")
	FOnReset OnReset;
};
