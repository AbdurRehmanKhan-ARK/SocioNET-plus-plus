# UIHelpers.h

## Component Name
UIHelpers

## Responsibilities
- Define console-based UI layout helpers for the SocioNET++ text interface.
- Provide common functions for screen clearing, box drawing, and aligned text output.
- Support user interaction formatting across multiple UI modules.

## Data Structures
- `const int BOX_WIDTH` — fixed width used for UI boxes.
- `std::string` label and text formatting parameters.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `void clearScreen()` | none | `void` | Clears the console screen via platform command. |
| `void printBoxHeader(const string& rawTitle)` | `rawTitle` | `void` | Draws a top border and centered title line. |
| `void printBoxSeparator()` | none | `void` | Draws an internal horizontal separator line. |
| `void printBoxFooter()` | none | `void` | Draws the bottom border of the UI box. |
| `void printPaddedLine(const string& rawLabel, const string& value = "")` | `rawLabel`, `value` | `void` | Prints a left-aligned padded content line. |
| `void printCenteredLine(const string& rawText)` | `rawText` | `void` | Prints centered text inside the UI box. |
| `void waitEnter()` | none | `void` | Waits for the user to press Enter. |

## Algorithmic Complexity
- All methods are O(L) where L is the length of the rendered string.
- Space is O(1) auxiliary; only temporary formatting state is used.

## Dependencies
- Standard library: `<iostream>`, `<string>`, `<iomanip>`, `<limits>`
