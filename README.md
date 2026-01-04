# PuzzleCore
`PuzzleCore` is a minimal, modular puzzle framework for `UnrealEngine`, designed to simplify implementing puzzle logic in both offline and online multiplayer games.

The system focuses on clear separation of logic, data-driven conditions, and network-friendly design, making it suitable for small indie projects as well as scalable multiplayer experiences.

## Concepts
1. PuzzleComponent
`UPuzzleComponent` is the main component you attach to an Actor, each puzzle is a component and when solved that all condition is true you can define condition witth `UPuzzleCheck` class
2. PuzzleCheck
`UPuzzleCheck` is a `UObject` calss that has 