// uiInterest.cpp
// Implementation of UI functions for displaying interests/posts
// Adapted to work with variant-based Interest system

#include "../include/UIInterest.h"
#include <variant>
#include <algorithm>
using std::vector;
using std::string;
using std::decay_t;
using std::is_same_v;
using std::visit;
using std::to_string;
using std::getline;
using std::holds_alternative;
using std::cout;
using std::cin;

// Helper to get type name from variant
string getInterestTypeName(const InterestVariant& interest) {
    return visit([](const auto& i) -> string {
        using T = decay_t<decltype(i)>;
        if constexpr (is_same_v<T, BookInterest>) return "BOOK";
        else if constexpr (is_same_v<T, MovieInterest>) return "MOVIE";
        else if constexpr (is_same_v<T, PlaceInterest>) return "PLACE";
        else if constexpr (is_same_v<T, FoodInterest>) return "FOOD";
        else if constexpr (is_same_v<T, SportInterest>) return "SPORT";
        else return "UNKNOWN";
    }, interest);
}

// Helper to check if interest is favorite
bool isFavoriteInterest(const InterestVariant& interest, const User& user) {
    return visit([&user](const auto& i) -> bool {
        using T = decay_t<decltype(i)>;
        if constexpr (is_same_v<T, BookInterest>) {
            const auto& fav = user.getFavoriteBook();
            return i.getTitle() == fav.getTitle() && i.getAuthor() == fav.getAuthor();
        }
        else if constexpr (is_same_v<T, MovieInterest>) {
            const auto& fav = user.getFavoriteMovie();
            return i.getTitle() == fav.getTitle() && i.getDirector() == fav.getDirector();
        }
        else if constexpr (is_same_v<T, PlaceInterest>) {
            const auto& fav = user.getFavoritePlace();
            return i.getName() == fav.getName();
        }
        else if constexpr (is_same_v<T, FoodInterest>) {
            const auto& fav = user.getFavoriteFood();
            return i.getDish() == fav.getDish();
        }
        else if constexpr (is_same_v<T, SportInterest>) {
            const auto& fav = user.getFavoriteSport();
            return i.getSport() == fav.getSport();
        }
        return false;
    }, interest);
}

void printDynamicGrid(const User& user) {
    const auto& stack = user.getRecentPosts().getStack();
    
    if (stack.empty()) {
        printBoxSeparator();
        printCenteredLine("No Posts Yet");
        return;
    }

    const int COLS = 3;
    const int CELL_WIDTH = 22;
    const int CELL_HEIGHT = 5;

    int totalPosts = stack.size();
    int rows = (totalPosts + COLS - 1) / COLS;

    vector<string> lines(rows * CELL_HEIGHT, string(COLS * (CELL_WIDTH + 3) + 1, ' '));

    auto setChar = [&](int r, int c, int lr, int lc, char ch) {
        int y = r * CELL_HEIGHT + lr;
        int x = c * (CELL_WIDTH + 3) + lc;
        if (y < (int)lines.size() && x < (int)lines[y].size())
            lines[y][x] = ch;
    };

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < COLS; ++c) {
            int postIdx = totalPosts - 1 - (r * COLS + c);
            bool hasPost = postIdx >= 0 && postIdx < totalPosts;

            // Draw cell borders
            for (int i = 0; i < CELL_WIDTH + 2; ++i) {
                setChar(r, c, 0, i + 1, '-');
                setChar(r, c, CELL_HEIGHT - 1, i + 1, '-');
            }
            for (int i = 0; i < CELL_HEIGHT; ++i) {
                setChar(r, c, i, 0, '|');
                setChar(r, c, i, CELL_WIDTH + 2, '|');
            }
            setChar(r, c, 0, 0, '+');
            setChar(r, c, 0, CELL_WIDTH + 2, '+');
            setChar(r, c, CELL_HEIGHT - 1, 0, '+');
            setChar(r, c, CELL_HEIGHT - 1, CELL_WIDTH + 2, '+');

            if (!hasPost) {
                string empty = "[ Empty Slot ]";
                int start = (CELL_WIDTH - (int)empty.size()) / 2;
                for (size_t i = 0; i < empty.size(); ++i)
                    setChar(r, c, 2, start + i + 1, empty[i]);
                continue;
            }

            // Get attributes based on variant type
            vector<string> attrs;
            visit([&attrs](const auto& p) {
                using T = decay_t<decltype(p)>;
                if constexpr (is_same_v<T, BookInterest>) {
                    attrs = { p.getTitle(), p.getAuthor(), to_string(p.getYear()) };
                }
                else if constexpr (is_same_v<T, MovieInterest>) {
                    attrs = { p.getTitle(), p.getDirector(), to_string(p.getYear()) };
                }
                else if constexpr (is_same_v<T, PlaceInterest>) {
                    attrs = { p.getName(), p.getCity(), p.getCountry() };
                }
                else if constexpr (is_same_v<T, FoodInterest>) {
                    attrs = { p.getDish(), p.getCuisine(), p.getPlace() };
                }
                else if constexpr (is_same_v<T, SportInterest>) {
                    attrs = { p.getSport(), p.getTeam1() + " Vs " + p.getTeam2(), p.getVenue() };
                }
            }, stack[postIdx]);

            // Truncate long strings
            for (auto& s : attrs)
                if (s.size() > CELL_WIDTH - 4)
                    s = s.substr(0, CELL_WIDTH - 7) + "...";

            // Center and place text
            for (int i = 0; i < (int)attrs.size() && i < 3; ++i) {
                string text = attrs[i];
                int start = (CELL_WIDTH - (int)text.size()) / 2;
                for (size_t j = 0; j < text.size(); ++j)
                    setChar(r, c, i + 1, start + j + 1, text[j]);
            }
        }
    }
    printBoxSeparator();
    printCenteredLine("Uploaded Posts (Latest First)");
    for (const auto& line : lines)
        cout << "║  " << line << "  ║\n";
    cout << "║";
}

void printInterestDetail(const InterestVariant& interest) {
    visit([](const auto& i) {
        using T = decay_t<decltype(i)>;
        if constexpr (is_same_v<T, BookInterest>) {
            cout << "╔════════════════════════════════════════════╗\n";
            cout << "║              BOOK POST                     ║\n";
            cout << "╠════════════════════════════════════════════╣\n";
            cout << "║ Title: " << i.getTitle() << "\n";
            cout << "║ Author: " << i.getAuthor() << "\n";
            cout << "║ Genre: " << i.getGenre() << "\n";
            cout << "║ Year: " << i.getYear() << "\n";
            cout << "║ Language: " << i.getLanguage() << "\n";
            cout << "║ Publisher: " << i.getPublisher() << "\n";
            cout << "╚════════════════════════════════════════════╝\n";
        }
        else if constexpr (is_same_v<T, MovieInterest>) {
            cout << "╔════════════════════════════════════════════╗\n";
            cout << "║              MOVIE POST                    ║\n";
            cout << "╠════════════════════════════════════════════╣\n";
            cout << "║ Title: " << i.getTitle() << "\n";
            cout << "║ Director: " << i.getDirector() << "\n";
            cout << "║ Genre: " << i.getGenre() << "\n";
            cout << "║ Year: " << i.getYear() << "\n";
            cout << "║ Language: " << i.getLanguage() << "\n";
            cout << "║ Production: " << i.getProduction() << "\n";
            cout << "╚════════════════════════════════════════════╝\n";
        }
        else if constexpr (is_same_v<T, PlaceInterest>) {
            cout << "╔════════════════════════════════════════════╗\n";
            cout << "║              PLACE POST                    ║\n";
            cout << "╠════════════════════════════════════════════╣\n";
            cout << "║ Name: " << i.getName() << "\n";
            cout << "║ City: " << i.getCity() << "\n";
            cout << "║ Country: " << i.getCountry() << "\n";
            cout << "║ Type: " << i.getType() << "\n";
            cout << "║ Visited: " << i.getVisitDate().toString() << "\n";
            cout << "╚════════════════════════════════════════════╝\n";
        }
        else if constexpr (is_same_v<T, FoodInterest>) {
            cout << "╔════════════════════════════════════════════╗\n";
            cout << "║              FOOD POST                     ║\n";
            cout << "╠════════════════════════════════════════════╣\n";
            cout << "║ Dish: " << i.getDish() << "\n";
            cout << "║ Cuisine: " << i.getCuisine() << "\n";
            cout << "║ Place: " << i.getPlace() << "\n";
            cout << "║ Ingredients: " << i.getIngredients() << "\n";
            cout << "║ Meal Type: " << i.getMeal() << "\n";
            cout << "╚════════════════════════════════════════════╝\n";
        }
        else if constexpr (is_same_v<T, SportInterest>) {
            cout << "╔════════════════════════════════════════════╗\n";
            cout << "║              SPORT POST                    ║\n";
            cout << "╠════════════════════════════════════════════╣\n";
            cout << "║ Sport: " << i.getSport() << "\n";
            cout << "║ League: " << i.getLeague() << "\n";
            cout << "║ Match: " << i.getTeam1() << " vs " << i.getTeam2() << "\n";
            cout << "║ Date: " << i.getEventDate().toString() << "\n";
            cout << "║ Venue: " << i.getVenue() << "\n";
            cout << "╚════════════════════════════════════════════╝\n";
        }
    }, interest);
}

void viewInterests(User& user) {
    while (true) {
        clearScreen();
        printBoxHeader("View Posts - @" + user.getUsername());
        printPaddedLine("1. View All Posts");
        printPaddedLine("2. View Book Posts");
        printPaddedLine("3. View Movie Posts");
        printPaddedLine("4. View Place Posts");
        printPaddedLine("5. View Food Posts");
        printPaddedLine("6. View Sport Posts");
        printPaddedLine("7. View Favorite Posts");
        printPaddedLine("0. Back To Profile");
        printBoxFooter();

        string choice;
        cout << "\n";
        bool cap = true;
        string prompt = "Enter Choice (0-7): ";
        for (char c : prompt) {
            if (cap && isalpha(c)) { cout << char(toupper(c)); cap = false; }
            else { cout << c; cap = isspace(c); }
        }
        getline(cin, choice);

        vector<InterestVariant> filtered;
        string title = "";

        if (choice == "1") {
            // View All Posts: use InterestStack as a stack (LIFO). Use a copy so original is unchanged.
            InterestStack stackCopy = user.getRecentPosts();
            title = "All Posts";
            while (!stackCopy.empty()) {
                InterestVariant *top = stackCopy.top();
                if (top != nullptr) filtered.push_back(*top);
                stackCopy.pop();
            }
        }
        else if (choice == "2") {
            // Use dedicated Book posts vector, newest first
            title = "Book Posts";
            const auto &books = user.getReadBooks();
            for (int i = static_cast<int>(books.size()) - 1; i >= 0; --i) {
                filtered.push_back(InterestVariant(books[i]));
            }
        }
        else if (choice == "3") {
            title = "Movie Posts";
            const auto &movies = user.getWatchedMovies();
            for (int i = static_cast<int>(movies.size()) - 1; i >= 0; --i) {
                filtered.push_back(InterestVariant(movies[i]));
            }
        }
        else if (choice == "4") {
            title = "Place Posts";
            const auto &places = user.getTravelledPlaces();
            for (int i = static_cast<int>(places.size()) - 1; i >= 0; --i) {
                filtered.push_back(InterestVariant(places[i]));
            }
        }
        else if (choice == "5") {
            title = "Food Posts";
            const auto &foods = user.getTriedFoods();
            for (int i = static_cast<int>(foods.size()) - 1; i >= 0; --i) {
                filtered.push_back(InterestVariant(foods[i]));
            }
        }
        else if (choice == "6") {
            title = "Sport Posts";
            const auto &sports = user.getAttendedSportsEvents();
            for (int i = static_cast<int>(sports.size()) - 1; i >= 0; --i) {
                filtered.push_back(InterestVariant(sports[i]));
            }
        }
        else if (choice == "7") {
            title = "Favorite Posts";
            const auto& favBook = user.getFavoriteBook();
            const auto& favMovie = user.getFavoriteMovie();
            const auto& favPlace = user.getFavoritePlace();
            const auto& favFood = user.getFavoriteFood();
            const auto& favSport = user.getFavoriteSport();
            
            // Check if favorites are set (non-empty)
            if (!favBook.getTitle().empty()) filtered.push_back(InterestVariant(favBook));
            if (!favMovie.getTitle().empty()) filtered.push_back(InterestVariant(favMovie));
            if (!favPlace.getName().empty()) filtered.push_back(InterestVariant(favPlace));
            if (!favFood.getDish().empty()) filtered.push_back(InterestVariant(favFood));
            if (!favSport.getSport().empty()) filtered.push_back(InterestVariant(favSport));
        }
        else if (choice == "0") break;
        else {
            clearScreen();
            printBoxHeader("Invalid Choice");
            printCenteredLine("Please Enter 0-7");
            printBoxFooter();
            waitEnter();
            continue;
        }

        if (filtered.empty()) {
            clearScreen();
            printBoxHeader(title);
            printCenteredLine("No Posts In This Category");
            printBoxFooter();
            waitEnter();
            continue;
        }

        // Scroll through each post
        for (size_t i = 0; i < filtered.size(); ++i) {
            clearScreen();
            printBoxHeader(title + " (" + to_string(i + 1) + "/" + to_string(filtered.size()) + ")");
            printInterestDetail(filtered[i]);
            waitEnter();
        }
    }
}

