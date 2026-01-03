// #pragma once

// #include "CoreMinimal.h"
// #include "UObject/Interface.h"
// #include "PuzzleInterface.generated.h"

// UINTERFACE(BlueprintType)
// class UPuzzleInterface : public UInterface
// {
//     GENERATED_BODY()
// };

// class IPuzzleInterface
// {
//     GENERATED_BODY()

// public:
//     /* Current puzzle state */
//     UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle|Getter")
//     EPuzzleState GetPuzzleState() const;
//     /* Is puzzle already solved */
//     UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle|Getter")
//     bool IsPuzzleSolved() const;
//     /* Is Puzzle Solved or Failed return true*/
//     UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle|Getter")
//     bool IsPuzzleUnlocked() const;
//     /* Is puzzle not be Unavailable */
//     UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle|Getter")
//     bool IsPuzzleActive() const;
//     /* Human-readable puzzle name */
//     UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle|Getter")
//     FName GetPuzzleName() const;
//     /* Attempt to solve the puzzle */
//     UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle")
//     bool TrySolve(UObject *Solver);
//     /* Can the puzzle currently be solved */
//     UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle")
//     bool CanSolve(UObject *Solver) const;
//     /* Number of attempts so far */
//     UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle|Getter")
//     int32 GetTryCount() const;
//     /*After puzzle faild with call this method reset the puzzle to Locked State*/
//     UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Puzzle")
//     void ResetPuzzle();
// };
