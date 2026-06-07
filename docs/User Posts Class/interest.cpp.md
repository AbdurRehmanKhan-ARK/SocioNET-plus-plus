# interest.cpp

## Component Name
Interest implementation

## Responsibilities
- Implement serialization and deserialization for all interest classes.
- Implement stack semantics and persistence for an `InterestStack`.
- Provide constructors for each interest type.

## Data Structures
- `InterestVariant` — variant subscribers for polymorphic data storage.
- `std::vector<InterestVariant> stack` — internal container for recent posts.
- Binary serialization buffers via `ostream` / `istream`.

## Key Methods
| Function | Parameters | Return Type | Description |
|---|---|---|---|
| `BookInterest::BookInterest(const std::string &title, const std::string &auth)` | `title`, `auth` | `BookInterest` | Initializes a book interest. |
| `void BookInterest::serialize(ostream &os) const` | `os` | `void` | Writes book interest fields to binary stream. |
| `void BookInterest::deserialize(istream &is)` | `is` | `void` | Reads book interest fields from binary stream. |
| `MovieInterest::MovieInterest(const std::string &title, const std::string &dir)` | `title`, `dir` | `MovieInterest` | Initializes a movie interest. |
| `PlaceInterest::PlaceInterest(const std::string &name)` | `name` | `PlaceInterest` | Initializes a place interest. |
| `FoodInterest::FoodInterest(const std::string &dish)` | `dish` | `FoodInterest` | Initializes a food interest. |
| `SportInterest::SportInterest(const std::string &sport, const Date &d)` | `sport`, `d` | `SportInterest` | Initializes a sport interest. |
| `void InterestStack::push(const InterestVariant& interest)` | `interest` | `void` | Pushes a new interest onto the stack. |
| `void InterestStack::pop()` | none | `void` | Pops the top interest if present. |
| `const vector<InterestVariant>& InterestStack::getStack() const` | none | reference | Returns stack contents. |
| `InterestVariant* InterestStack::top()` | none | `InterestVariant*` | Returns pointer to the top interest or `nullptr`. |
| `bool InterestStack::empty() const` | none | `bool` | Checks if the stack is empty. |
| `void InterestStack::printTop() const` | none | `void` | Prints a simple summary of the top post. |
| `void InterestStack::serialize(ostream &os) const` | `os` | `void` | Serializes stack entries with type tags. |
| `void InterestStack::deserialize(istream &is)` | `is` | `void` | Deserializes stack entries from binary data. |

## Algorithmic Complexity
- Individual interest serialization — O(1) per field.
- `InterestStack::push()` / `pop()` / `top()` — O(1).
- `InterestStack::serialize()` / `deserialize()` — Time: O(N), Space: O(N) where N = number of stack entries.

## Dependencies
- `Interest.h`
- `Helper.h`
- Standard library: `<iostream>`
