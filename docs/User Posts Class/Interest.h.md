# Interest.h

## Component Name
Interest definitions

## Responsibilities
- Declare structured interest types for books, movies, places, food, and sports.
- Define serialization interfaces for interest persistence.
- Provide a variant-based stack for recent posts.

## Data Structures
- `BookInterest`, `MovieInterest`, `PlaceInterest`, `FoodInterest`, `SportInterest` — lightweight classes representing different interest domains.
- `InterestVariant` — `std::variant` over the five interest types.
- `InterestStack` — wrapper around `std::vector<InterestVariant>`.
- `Date` — shared date type used by travel and sport interests.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `BookInterest()` | none | `BookInterest` | Constructs an empty book preference. |
| `BookInterest(const string &title, const string &auth)` | `title`, `auth` | `BookInterest` | Constructs book interest with title and author. |
| `void serialize(ostream &os) const` | `os` | `void` | Writes book fields to a binary stream. |
| `void deserialize(istream &is)` | `is` | `void` | Reads book data from a binary stream. |
| `MovieInterest(const string &title, const string &dir)` | `title`, `dir` | `MovieInterest` | Constructs movie interest. |
| `PlaceInterest(const string &name)` | `name` | `PlaceInterest` | Constructs place interest. |
| `FoodInterest(const string &dish)` | `dish` | `FoodInterest` | Constructs food interest. |
| `SportInterest(const string &sport, const Date &d)` | `sport`, `d` | `SportInterest` | Constructs sport interest with event date. |
| `void push(const InterestVariant& interest)` | `interest` | `void` | Adds a post to the stack. |
| `void pop()` | none | `void` | Removes the top post if present. |
| `const vector<InterestVariant>& getStack() const` | none | reference | Returns stack contents. |
| `InterestVariant* top()` | none | `InterestVariant*` | Returns mutable top post pointer or `nullptr`. |
| `bool empty() const` | none | `bool` | Checks whether the stack is empty. |

## Algorithmic Complexity
- `serialize()` / `deserialize()` for interests — Time: O(F) where F = field count, Space: O(1) auxiliary.
- `InterestStack::push()` / `pop()` / `top()` — Time: O(1), Space: O(1) amortized.
- `InterestStack::serialize()` / `deserialize()` — Time: O(N) where N = number of stack entries, Space: O(N) for stack storage.

## Dependencies
- `Helper.h` for `Date`
- Standard library: `<string>`, `<vector>`, `<variant>`, `<istream>`, `<ostream>`, `<cstdint>`
