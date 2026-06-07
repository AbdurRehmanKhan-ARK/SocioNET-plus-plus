# uiInterest.cpp

## Component Name
UIInterest implementation

## Responsibilities
- Implement interactive interest viewing and display logic.
- Render dynamic grids for recent posts and detailed interest views.
- Support filtered browsing by interest category and favorites.

## Data Structures
- `std::vector<InterestVariant>` for filtered post lists.
- `InterestStack` copies for safe view-only traversal.
- `std::string` values for formatted display fields.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `string getInterestTypeName(const InterestVariant& interest)` | `interest` | `string` | Returns a canonical type label for a variant. |
| `bool isFavoriteInterest(const InterestVariant& interest, const User& user)` | `interest`, `user` | `bool` | Determines whether the variant matches a favorite interest. |
| `void printDynamicGrid(const User& user)` | `user` | `void` | Renders the user’s recent posts in a three-column grid layout. |
| `void printInterestDetail(const InterestVariant& interest)` | `interest` | `void` | Renders a detailed view of a single interest entry. |
| `void viewInterests(User& user)` | `user` | `void` | Displays an interest menu and navigates to filtered views. |

## Algorithmic Complexity
- `printDynamicGrid()` — Time: O(P) for P posts, Space: O(P) for cell line buffers.
- `printInterestDetail()` — Time: O(1), Space: O(1).
- `viewInterests()` — Time: O(P) for filtering and display selections.

## Dependencies
- `UIInterest.h`
- `UIHelpers.h`
- `Interest.h`
- `User.h`
- Standard library: `<variant>`, `<algorithm>`, `<string>`
