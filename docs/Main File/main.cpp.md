# main.cpp

## Component Name
Application entry point

## Responsibilities
- Initialize runtime state for email lookup, friend graph, and username search.
- Display the main menu and route registration/sign-in commands.
- Manage application lifecycle and cleanup global resources.

## Data Structures
- `Authentication auth` — authentication controller.
- `std::string choice` — menu selection input.
- `User currentUser` — currently signed-in user object.
- `FileManager fm` — file persistence utility for graph loading.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `int main()` | none | `int` | Program entry point; initializes subsystems and runs the main CLI loop. |

## Algorithmic Complexity
- Startup initialization is O(U + V + E) where U = number of usernames, V/E = graph size.
- Menu loop operations are O(1) per displayed option, with backend actions adding their own costs.

## Dependencies
- `Authentication.h`
- `UIHelpers.h`
- `UIUser.h`
- `FileManager.h`
- `Graph.h`
- Standard library: `<windows.h>`, `<iostream>`
