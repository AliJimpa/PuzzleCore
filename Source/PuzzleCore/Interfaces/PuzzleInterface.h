#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PuzzleInterface.generated.h"

// UINTERFACE is required, but no Blueprint exposure
UINTERFACE()
class PUZZLECORE_API UPuzzleInterface : public UInterface
{
    GENERATED_BODY()
};

// Pure C++ interface
class PUZZLECORE_API IPuzzleInterface
{
    GENERATED_BODY()

public:
    // Current puzzle state
    virtual EPuzzleState GetPuzzleState() const = 0;

    // Is puzzle already solved
    virtual bool IsPuzzleSolved() const = 0;

    // Is Puzzle Solved or Failed
    virtual bool IsPuzzleUnlocked() const = 0;

    // Is puzzle available / active
    virtual bool IsPuzzleActive() const = 0;

    // Human-readable puzzle name
    virtual FName GetPuzzleName() const = 0;

    // Attempt to solve the puzzle
    virtual bool TrySolve(UObject* Solver) = 0;

    // Can the puzzle currently be solved
    virtual bool CanSolve(UObject* Solver) const = 0;

    // Number of attempts so far
    virtual int32 GetTryCount() const = 0;

    // Reset the puzzle to locked state
    virtual void ResetPuzzle() = 0;
};
