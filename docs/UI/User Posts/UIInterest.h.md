# UIInterest.h

## Component Name
UIInterest

## Responsibilities
- Declare UI rendering helpers for interest posts and favorites.
- Provide functions for filtering, viewing, and formatting interest data.
- Support variant-based interest display workflows.

## Data Structures
- `InterestVariant` — union of all interest types used for display.
- `User` reference for current profile interests.
- `std::vector<string>` for filtered and formatted interest listings.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `void printDynamicGrid(const User& user)` | `user` | `void` | Displays recent posts in a three-column grid. |
| `void printInterestDetail(const InterestVariant& interest)` | `interest` | `void` | Renders a detailed view of a single interest entry. |
| `void viewInterests(User& user)` | `user` | `void` | Offers a menu to view all or filtered post categories. |
| `void viewInterestsByType(User& user, const string& typeChoice)` | `user`, `typeChoice` | `void` | Displays posts filtered by type string. |
| `void viewFavoriteInterests(const User& user)` | `user` | `void` | Lists the user's favorite interests. |
| `string getInterestTypeName(const InterestVariant& interest)` | `interest` | `string` | Returns a human-readable interest type name. |
| `bool isFavoriteInterest(const InterestVariant& interest, const User& user)` | `interest`, `user` | `bool` | Determines if an interest matches the user's favorite. |

## Algorithmic Complexity
- `printDynamicGrid()` — Time: O(P) where P = number of posts displayed, Space: O(P) for rendered lines.
- `printInterestDetail()` — Time: O(1), Space: O(1).
- `viewInterests()` and other menu flows — Time: O(P) for filtering and display, with P = selected posts.

## Dependencies
- `User.h`
- `Interest.h`
- `UIHelpers.h`
- Standard library: `<vector>`, `<string>`
