# uiHelpers.cpp

## Component Name
UIHelpers implementation

## Responsibilities
- Implement console drawing and formatting utilities for the text UI.
- Enable UTF-8 title display and padded line rendering.
- Provide user interaction helpers such as wait-for-enter.

## Data Structures
- `const int BOX_WIDTH` — configured box width of 80 characters.
- Formatted strings built for console output.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `void clearScreen()` | none | `void` | Clears the Windows console. |
| `void printBoxHeader(const string& rawTitle)` | `rawTitle` | `void` | Renders a top border and centered title. |
| `void printBoxSeparator()` | none | `void` | Prints a separator row within the UI box. |
| `void printBoxFooter()` | none | `void` | Prints the bottom border of the box. |
| `void printPaddedLine(const string& rawLabel, const string& value)` | `rawLabel`, `value` | `void` | Renders a label/value line with padding. |
| `void printCenteredLine(const string& rawText)` | `rawText` | `void` | Renders centered text inside the current box width. |
| `void waitEnter()` | none | `void` | Pauses execution until Enter is pressed. |

## Algorithmic Complexity
- All methods are O(L) where L is the length of the rendered text.
- No significant auxiliary memory is allocated beyond temporary formatted strings.

## Dependencies
- `UIHelpers.h`
- Standard library: `<algorithm>`, `<cctype>`, `<windows.h>`, `<iostream>`, `<limits>`
