// uiUser.cpp
// Implementation of UI functions for user profile, dashboard, and post management
// Adapted to work with backend User class and variant-based Interest system

#include "../include/UIUser.h"
#include "../include/Authentication.h"
#include "../include/FileManager.h"
#include "../include/Helper.h"
#include "../include/Graph.h"
#include <variant>
#include <algorithm>
#include <iomanip>
using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::getline;
using std::to_string;
using std::decay_t;
using std::is_same_v;
using std::visit;
using std::setprecision;
using std::fixed;

void addInterest(User& user) {
    clearScreen();
    printBoxHeader("Add New Post");
    printCenteredLine("Choose Post Type");
    printBoxSeparator();
    printPaddedLine("1. Book");
    printPaddedLine("2. Movie");
    printPaddedLine("3. Place");
    printPaddedLine("4. Food");
    printPaddedLine("5. Sport");
    printBoxFooter();

    string choice;
    cout << "\nChoice: ";
    getline(cin, choice);

    InterestVariant newInterest;
    bool validChoice = false;

    if (choice == "1") {
        BookInterest b;
        clearScreen();
        printBoxHeader("Enter Your Book Post Details");
        string title, author, genre, language, publisher, yearStr;
        cout << "║ Title: "; getline(cin, title);
        cout << "║ Author: "; getline(cin, author);
        cout << "║ Genre: "; getline(cin, genre);
        cout << "║ Year: "; getline(cin, yearStr);
        cout << "║ Language: "; getline(cin, language);
        cout << "║ Publisher: "; getline(cin, publisher);
        b.setTitle(title);
        b.setAuthor(author);
        b.setGenre(genre);
        b.setYear(stoi(yearStr.empty() ? "0" : yearStr));
        b.setLanguage(language);
        b.setPublisher(publisher);
        newInterest = InterestVariant(b);
        validChoice = true;
        
        // Ask if favorite
        cout << "║ Is It Your Favorite Book? (1 -> Yes | 0 -> No): ";
        int isFav; cin >> isFav; cin.ignore();
        if (isFav == 1) {
            // Check if already has favorite
            if (!user.getFavoriteBook().getTitle().empty()) {
                cout << "║ You Already Have a Favorite Book! Want to Replace? (1=Yes/0=No): ";
                int rep; cin >> rep; cin.ignore();
                if (rep == 1) {
                    user.getFavoriteBook() = b;
                }
            } else {
                user.getFavoriteBook() = b;
            }
        }
    }
    else if (choice == "2") {
        MovieInterest m;
        clearScreen();
        printBoxHeader("Enter Your Movie Post Details");
        string title, director, genre, language, production, yearStr;
        cout << "║ Title: "; getline(cin, title);
        cout << "║ Director: "; getline(cin, director);
        cout << "║ Genre: "; getline(cin, genre);
        cout << "║ Year: "; getline(cin, yearStr);
        cout << "║ Language: "; getline(cin, language);
        cout << "║ Production: "; getline(cin, production);
        m.setTitle(title);
        m.setDirector(director);
        m.setGenre(genre);
        m.setYear(stoi(yearStr.empty() ? "0" : yearStr));
        m.setLanguage(language);
        m.setProduction(production);
        newInterest = InterestVariant(m);
        validChoice = true;
        
        cout << "║ Is It Your Favorite Movie? (1 -> Yes | 0 -> No): ";
        int isFav; cin >> isFav; cin.ignore();
        if (isFav == 1) {
            if (!user.getFavoriteMovie().getTitle().empty()) {
                cout << "║ You Already Have a Favorite Movie! Want to Replace? (1=Yes/0=No): ";
                int rep; cin >> rep; cin.ignore();
                if (rep == 1) {
                    user.getFavoriteMovie() = m;
                }
            } else {
                user.getFavoriteMovie() = m;
            }
        }
    }
    else if (choice == "3") {
        PlaceInterest p;
        clearScreen();
        printBoxHeader("Enter Your Place Post Details");
        string name, city, country, type, d, m, y;
        cout << "║ Name: "; getline(cin, name);
        cout << "║ City: "; getline(cin, city);
        cout << "║ Country: "; getline(cin, country);
        cout << "║ Type: "; getline(cin, type);
        cout << "║ Visit Day: "; getline(cin, d);
        cout << "║ Visit Month: "; getline(cin, m);
        cout << "║ Visit Year: "; getline(cin, y);
        p.setName(name);
        p.setCity(city);
        p.setCountry(country);
        p.setType(type);
        Date visitDate(stoi(d.empty() ? "1" : d), stoi(m.empty() ? "1" : m), stoi(y.empty() ? "2000" : y));
        p.setVisitDate(visitDate);
        newInterest = InterestVariant(p);
        validChoice = true;
        
        cout << "║ Is It Your Favorite Place? (1 -> Yes | 0 -> No): ";
        int isFav; cin >> isFav; cin.ignore();
        if (isFav == 1) {
            if (!user.getFavoritePlace().getName().empty()) {
                cout << "║ You Already Have a Favorite Place! Want to Replace? (1=Yes/0=No): ";
                int rep; cin >> rep; cin.ignore();
                if (rep == 1) {
                    user.getFavoritePlace() = p;
                }
            } else {
                user.getFavoritePlace() = p;
            }
        }
    }
    else if (choice == "4") {
        FoodInterest f;
        clearScreen();
        printBoxHeader("Enter Your Food Post Details");
        string dish, cuisine, place, ingredients, meal;
        cout << "║ Dish: "; getline(cin, dish);
        cout << "║ Cuisine: "; getline(cin, cuisine);
        cout << "║ Place: "; getline(cin, place);
        cout << "║ Ingredients: "; getline(cin, ingredients);
        cout << "║ Meal: "; getline(cin, meal);
        f.setDish(dish);
        f.setCuisine(cuisine);
        f.setPlace(place);
        f.setIngredients(ingredients);
        f.setMeal(meal);
        newInterest = InterestVariant(f);
        validChoice = true;
        
        cout << "║ Is It Your Favorite Food? (1 -> Yes | 0 -> No): ";
        int isFav; cin >> isFav; cin.ignore();
        if (isFav == 1) {
            if (!user.getFavoriteFood().getDish().empty()) {
                cout << "║ You Already Have a Favorite Food! Want to Replace? (1=Yes/0=No): ";
                int rep; cin >> rep; cin.ignore();
                if (rep == 1) {
                    user.getFavoriteFood() = f;
                }
            } else {
                user.getFavoriteFood() = f;
            }
        }
    }
    else if (choice == "5") {
        SportInterest s;
        clearScreen();
        printBoxHeader("Enter Your Sport Post Details");
        string sport, league, team1, team2, venue, d, m, y;
        cout << "║ Sport: "; getline(cin, sport);
        cout << "║ League: "; getline(cin, league);
        cout << "║ Team 1: "; getline(cin, team1);
        cout << "║ Team 2: "; getline(cin, team2);
        cout << "║ Venue: "; getline(cin, venue);
        cout << "║ Event Day: "; getline(cin, d);
        cout << "║ Event Month: "; getline(cin, m);
        cout << "║ Event Year: "; getline(cin, y);
        s.setSport(sport);
        s.setLeague(league);
        s.setTeam1(team1);
        s.setTeam2(team2);
        s.setVenue(venue);
        Date eventDate(stoi(d.empty() ? "1" : d), stoi(m.empty() ? "1" : m), stoi(y.empty() ? "2000" : y));
        s.setEventDate(eventDate);
        newInterest = InterestVariant(s);
        validChoice = true;
        
        cout << "║ Is It Your Favorite Sport Event? (1 -> Yes | 0 -> No): ";
        int isFav; cin >> isFav; cin.ignore();
        if (isFav == 1) {
            if (!user.getFavoriteSport().getSport().empty()) {
                cout << "║ You Already Have a Favorite Sport Event! Want to Replace? (1=Yes/0=No): ";
                int rep; cin >> rep; cin.ignore();
                if (rep == 1) {
                    user.getFavoriteSport() = s;
                }
            } else {
                user.getFavoriteSport() = s;
            }
        }
    }
    else {
        clearScreen();
        printBoxHeader("Invalid Choice");
        printCenteredLine("Post Not Added");
        printBoxFooter();
        waitEnter();
        return;
    }

    if (validChoice) {
        // Add to recent posts stack
        user.getRecentPosts().push(newInterest);
        
        // Also add to appropriate category vector
        visit([&user](auto& interest) {
            using T = decay_t<decltype(interest)>;
            if constexpr (is_same_v<T, BookInterest>) {
                user.getReadBooks().push_back(interest);
            }
            else if constexpr (is_same_v<T, MovieInterest>) {
                user.getWatchedMovies().push_back(interest);
            }
            else if constexpr (is_same_v<T, PlaceInterest>) {
                user.getTravelledPlaces().push_back(interest);
            }
            else if constexpr (is_same_v<T, FoodInterest>) {
                user.getTriedFoods().push_back(interest);
            }
            else if constexpr (is_same_v<T, SportInterest>) {
                user.getAttendedSportsEvents().push_back(interest);
            }
        }, newInterest);
        
        // Save user to file after adding post
        FileManager fileManager;
        if (fileManager.updateUser(user)) {
            clearScreen();
            printBoxHeader("Post Added Successfully");
            printCenteredLine("Your Post has Been Added");
            printBoxFooter();
            waitEnter();
        } else {
            clearScreen();
            printBoxHeader("Error");
            printCenteredLine("Failed to Save Post. Please Try Again.");
            printBoxFooter();
            waitEnter();
        }
    }
}

void editProfile(User& currentUser) {
    while (true) {
        clearScreen();
        printBoxHeader("Edit Profile - @" + currentUser.getUsername());
        printPaddedLine("1. Edit Full Name");
        printPaddedLine("2. Edit Gender");
        printPaddedLine("3. Edit Age");
        printPaddedLine("4. Edit Occupation");
        printPaddedLine("5. Edit Bio");
        printPaddedLine("6. Edit City");
        printPaddedLine("7. Edit Country");
        printPaddedLine("8. Edit Birth Date");
        printPaddedLine("0. Back to Profile");
        printBoxFooter();
        string choice;
        cout << "\nChoice: ";
        getline(cin, choice);
        
        if (choice == "1") {
            clearScreen();
            printBoxHeader("Edit Full Name");
            cout << "║ Current: " << (currentUser.getName().empty() ? "[Not Set]" : currentUser.getName()) << "\n";
            cout << "║ New: ";
            string newName;
            getline(cin, newName);
            bool cap = true;
            for (char& c : newName) {
            if (cap && isalpha(c)) { c = toupper(c); cap = false; }
            else cap = isspace(c);
            }
            currentUser.setName(newName);
        }
        else if (choice == "2") {
            clearScreen();
            printBoxHeader("Edit Gender");
            cout << "║ Current: " << (currentUser.getGender().empty() ? "[Not Set]" : currentUser.getGender()) << "\n";
            cout << "║ New (Male/Female/Other): ";
            string newGender;
            getline(cin, newGender);
            bool cap = true;
            for (char& c : newGender) {
            if (cap && isalpha(c)) { c = toupper(c); cap = false; }
            else cap = isspace(c);
            }
            currentUser.setGender(newGender);
        }
        else if (choice == "3") {
            clearScreen();
            printBoxHeader("Edit Age");
            cout << "║ Current: " << (currentUser.getAge().empty() ? "[Not Set]" : currentUser.getAge()) << "\n";
            cout << "║ New: ";
            string newAge;
            getline(cin, newAge);
            currentUser.setAge(newAge);
        }
        else if (choice == "4") {
            clearScreen();
            printBoxHeader("Edit Occupation");
            cout << "║ Current: " << (currentUser.getOccupation().empty() ? "[Not Set]" : currentUser.getOccupation()) << "\n";
            cout << "║ New: ";
            string newOcc;
            getline(cin, newOcc);
            bool cap = true;
            for (char& c : newOcc) {
            if (cap && isalpha(c)) { c = toupper(c); cap = false; }
            else cap = isspace(c);
            }
            currentUser.setOccupation(newOcc);
        }
        else if (choice == "5") {
            clearScreen();
            printBoxHeader("Edit Bio");
            cout << "║ Current: " << (currentUser.getBio().empty() ? "[Not Set]" : currentUser.getBio()) << "\n";
            cout << "║ New: ";
            string newBio;
            getline(cin, newBio);
            currentUser.setBio(newBio);
        }
        else if (choice == "6") {
            clearScreen();
            printBoxHeader("Edit City");
            cout << "║ Current: " << (currentUser.getCity().empty() ? "[Not Set]" : currentUser.getCity()) << "\n";
            cout << "║ New: ";
            string newCity;
            getline(cin, newCity);
            bool cap = true;
            for (char& c : newCity) {
            if (cap && isalpha(c)) { c = toupper(c); cap = false; }
            else cap = isspace(c);
            }
            currentUser.setCity(newCity);
        }
        else if (choice == "7") {
            clearScreen();
            printBoxHeader("Edit Country");
            cout << "║ Current: " << (currentUser.getCountry().empty() ? "[Not Set]" : currentUser.getCountry()) << "\n";
            cout << "║ New: ";
            string newCountry;
            getline(cin, newCountry);
            bool cap = true;
            for (char& c : newCountry) {
            if (cap && isalpha(c)) { c = toupper(c); cap = false; }
            else cap = isspace(c);
            }
            currentUser.setCountry(newCountry);
        }
        else if (choice == "8") {
            clearScreen();
            printBoxHeader("Edit Birth Date");
            Date currentDate = currentUser.getBirthDate();
            cout << "║ Current: " << currentDate.toString() << "\n";
            string d, m, y;
            cout << "║ Day: "; getline(cin, d);
            cout << "║ Month: "; getline(cin, m);
            cout << "║ Year: "; getline(cin, y);
            Date newDate(stoi(d.empty() ? "0" : d), stoi(m.empty() ? "0" : m), stoi(y.empty() ? "0" : y));
            currentUser.setBirthDate(newDate);
        }
        else if (choice == "0") break;
        else {
            clearScreen();
            printBoxHeader("Invalid Choice");
            printCenteredLine("Enter 0-8");
            printBoxFooter();
            waitEnter();
            continue;
        }
        
        // Save user to file after editing profile
        FileManager fileManager;
        if (fileManager.updateUser(currentUser)) {
            clearScreen();
            printBoxHeader("Updated");
            printCenteredLine("Field Updated Successfully!");
            printBoxFooter();
            waitEnter();
        } else {
            clearScreen();
            printBoxHeader("Error");
            printCenteredLine("Failed to Save Changes. Please Try Again.");
            printBoxFooter();
            waitEnter();
        }
    }
}

bool deleteProfile(User& currentUser) {
    clearScreen();
    printBoxHeader("Danger Zone - Delete Profile");
    printCenteredLine("Are you sure you want to delete your account?");
    printCenteredLine("This action can't be undone.");
    printBoxSeparator();
    cout << "║ Confirm (Y/N): ";
    string confirm;
    getline(cin, confirm);

    if (confirm.empty() || (confirm != "Y" && confirm != "y")) {
        clearScreen();
        printBoxHeader("Cancelled");
        printCenteredLine("Account deletion cancelled.");
        printBoxFooter();
        waitEnter();
        return false;
    }

    string password;
    clearScreen();
    printBoxHeader("Confirm Password");
    printCenteredLine("Enter your password to delete your account.");
    printBoxSeparator();
    cout << "║ Password: ";
    getline(cin, password);

    Authentication auth;
    string errorMsg;
    if (!auth.deleteUserAccount(currentUser, password, errorMsg)) {
        clearScreen();
        printBoxHeader("Deletion Failed");
        printCenteredLine(errorMsg.empty() ? "Password mismatch or deletion error." : errorMsg);
        printBoxFooter();
        waitEnter();
        return false;
    }

    currentUser = User();
    clearScreen();
    printBoxHeader("Account Deleted");
    printCenteredLine("Your account has been removed successfully.");
    printCenteredLine("You have been logged out.");
    printBoxFooter();
    waitEnter();
    return true;
}

void viewProfile(User& currentUser) {
    while(true){
        clearScreen();
        printBoxHeader("Profile | @" + currentUser.getUsername());
        
        printCenteredLine(currentUser.getName().empty() ? "No Name Set" : currentUser.getName());
        printBoxSeparator();
        
        printPaddedLine("Email: ", currentUser.getEmail());
        printPaddedLine("Gender: ", currentUser.getGender().empty() ? "[Not Set Yet]" : currentUser.getGender());
        printPaddedLine("Age: ", currentUser.getAge().empty() ? "[Not Set Yet]" : currentUser.getAge());
        printPaddedLine("Occupation: ", currentUser.getOccupation().empty() ? "[Not Set Yet]" : currentUser.getOccupation());
        printPaddedLine("Bio: ", currentUser.getBio().empty() ? "[Not Set Yet]" : currentUser.getBio());
        printPaddedLine("City: ", currentUser.getCity().empty() ? "[Not Set Yet]" : currentUser.getCity());
        printPaddedLine("Country: ", currentUser.getCountry().empty() ? "[Not Set Yet]" : currentUser.getCountry());
    
        Date birthDate = currentUser.getBirthDate();
        string birthStr = (birthDate.getDay() == 0 && birthDate.getMonth() == 0 && birthDate.getYear() == 0)
                          ? "[Not Set]"
                          : birthDate.toString();
        printPaddedLine("Birth Date: ", birthStr);
        printBoxSeparator();
        
        printPaddedLine("Posts: ", to_string(currentUser.getRecentPosts().size()));
        // Friends count from graph
        if (UserHelper::friend_graph != nullptr) {
            auto friends = UserHelper::friend_graph->getFriends(currentUser.getUsername());
            printPaddedLine("Friends: ", to_string(friends.size()));
        } else {
            printPaddedLine("Friends: ", "0"); // Graph not initialized
        }
        printBoxSeparator();

        double pc = currentUser.profileCompletion();
        int filledCount = static_cast<int>(pc / 12.5 + 0.0001);
        printPaddedLine("Profile Completion Bar ", "");

        cout << "║ ";
        for (int i = 0; i < 8; ++i) cout << (i < filledCount ? "█" : "░");
        cout << " " << fixed << setprecision(2) << pc << "%";
        int barLen = 8 + 1 + 4;
        for (int i = 0; i < BOX_WIDTH - barLen - 3; ++i) cout << " ";
        if(filledCount == 0){
            cout << " ║\n";
        }
        else if(filledCount == 8){
            cout << "\b║\n";
        }
        else{
            cout << "║\n";
        }

        printDynamicGrid(currentUser);
        printBoxSeparator();

        printPaddedLine("1. Edit Profile");
        printPaddedLine("2. View Posts");
        printPaddedLine("3. View My Friend Requests");
        printPaddedLine("4. View My Friends");
        printPaddedLine("5. Show Friend Suggestions");
        printPaddedLine("0. Back");
        printBoxFooter();

        string choice;
        cout << "\nEnter Your Choice: ";
        getline(cin, choice);
        if (choice == "1") editProfile(currentUser);
        else if (choice == "2") viewInterests(currentUser);
        else if (choice == "3") viewFriendRequests(currentUser);
        else if (choice == "4") viewMyFriends(currentUser);
        else if (choice == "5") showFriendSuggestions(currentUser);
        else if (choice == "0") break;
    }
}

void dashboard(User& currentUser) {
    while (true) {
        clearScreen();
        printBoxHeader("Dashboard Of User @" + currentUser.getUsername());
        printPaddedLine("1. View Profile");
        printPaddedLine("2. Edit Profile");
        printPaddedLine("3. Add Post");
        printPaddedLine("4. View Posts");
        printPaddedLine("5. Search User");
        printPaddedLine("6. Sign Out");
        printPaddedLine("0. Delete Profile");
        printBoxFooter();
        cout << "\n";
        cout << "Enter Your Choice:";
        string choice;
        getline(cin, choice);
        if (choice == "1") {
            viewProfile(currentUser);
        }
        else if (choice == "2") {
            editProfile(currentUser);
        }
        else if (choice == "3") {
            addInterest(currentUser);
        }
        else if (choice == "4") {
            viewInterests(currentUser);
        }
        else if (choice == "5") {
            searchUser(currentUser);
        }
        else if (choice == "0") {
            if (deleteProfile(currentUser)) {
                break;
            }
        }
        else if (choice == "6") {
            clearScreen();
            printBoxHeader("Sign Out");
            printCenteredLine("Thank You For Using SocioNET++");
            printBoxFooter();
            waitEnter();
            break;
        }
    }
}

// Search for a user by username using Suffix Automaton substring search
void searchUser(User& currentUser) {
    clearScreen();
    printBoxHeader("Search User");
    printCenteredLine("Enter Username Pattern To Search");
    printBoxSeparator();
    string pattern;
    cout << "Search: ";
    getline(cin, pattern);
    
    if (pattern.empty()) {
        clearScreen();
        printBoxHeader("Search Result");
        printCenteredLine("Pattern Cannot Be Empty");
        printBoxFooter();
        waitEnter();
        return;
    }
    
    // Use Suffix Automaton (shared) to search
    vector<int> matchedIDs = UserHelper::sam.search(pattern);

    if(!matchedIDs.empty()){
    // Remove current user from results if present
        auto it = std::remove_if(matchedIDs.begin(), matchedIDs.end(),
                                [&currentUser](int id) {
                                    return UserHelper::all_usernames[id] == currentUser.getUsername();
                                });
        matchedIDs.erase(it, matchedIDs.end());
    }
    
    if (matchedIDs.empty()) {
        clearScreen();
        printBoxHeader("Search Result");
        printCenteredLine("No Matching Users Found");
        printBoxFooter();
        waitEnter();
        return;
    }
    
    // Display matched usernames
    clearScreen();
    printBoxHeader("Search Results for \"" + pattern + "\"");
    printBoxSeparator();
    
    for (size_t i = 0; i < matchedIDs.size(); ++i) {
        int id = matchedIDs[i];
        if (id >= 0 && id < static_cast<int>(UserHelper::all_usernames.size())) {
            string line = to_string(i + 1) + ". @" + UserHelper::all_usernames[id];
            printPaddedLine(line);
        }
    }
    
    printBoxSeparator();
    cout << "║ Enter Number to Visit User (0 to Cancel): ";
    string choiceStr;
    getline(cin, choiceStr);
    
    int choice;
    try {
        choice = stoi(choiceStr);
    } catch (...) {
        clearScreen();
        printBoxHeader("Invalid Input");
        printCenteredLine("Please Enter A Valid Number");
        printBoxFooter();
        waitEnter();
        return;
    }
    
    if (choice == 0) {
        return;  // Cancel search
    }
    
    if (choice < 1 || choice > static_cast<int>(matchedIDs.size())) {
        clearScreen();
        printBoxHeader("Invalid Selection");
        printCenteredLine("Number Out Of Range");
        printBoxFooter();
        waitEnter();
        return;
    }
    
    // Get selected user
    int selectedID = matchedIDs[choice - 1];
    string targetUsername = UserHelper::all_usernames[selectedID];
    
    // Check if trying to view own profile
    // No need because we have already removed current user from results
    // But just in case, we re-check, otherwise this block can be omitted
    if (targetUsername == currentUser.getUsername()) {
        clearScreen();
        printBoxHeader("Cannot View Own Profile Here");
        printCenteredLine("Use 'View Profile' from Dashboard Instead");
        printBoxFooter();
        waitEnter();
        return;
    }
    
    // Load and view the selected user's profile
    FileManager fileManager;
    User foundUser = fileManager.getUserByUsername(targetUsername);
    
    if (foundUser.getUsername().empty()) {
        clearScreen();
        printBoxHeader("Error");
        printCenteredLine("User Data Could Not Be Loaded");
        printBoxFooter();
        waitEnter();
        return;
    }
    
    // Display the found user's profile
    viewOtherProfile(currentUser, foundUser);
}

void viewFriends(const User& user) {
    clearScreen();
    printBoxHeader("Friends of @" + user.getUsername());
    printCenteredLine("Friend Feature - Requires Graph Implementation");
    printBoxFooter();
    waitEnter();
}

void viewFriendRequests(User& currentUser)  // Function receives the logged-in user by reference (so changes are saved)
{
    while (true)  // Outer loop: keeps refreshing the request list until the user presses 0 to go back
    {
        clearScreen();  // Clears the console screen for a clean, fresh UI every time

        // Prints the top border + title with the current user's username
        printBoxHeader("Friend Requests for @" + currentUser.getUsername());

        // Build incoming requests list by scanning graph
        vector<string> incoming;
        if (UserHelper::friend_graph != nullptr) {
            const auto& adj = UserHelper::friend_graph->getAdjacencyList();
            for (const auto& pair : adj) {
                const string& other = pair.first;
                for (const Relation& rel : pair.second) {
                    if (rel.toUser == currentUser.getUsername() && rel.status == REQUEST_SENT) {
                        incoming.push_back(other);
                    }
                }
            }
        }

        // Check if there are no incoming friend requests
        if (incoming.empty())
        {
            // Show a centered message inside the box
            printCenteredLine("No Pending Requests");
            // Print the bottom border to close the box
            printBoxFooter();
            // Wait for the user to press Enter so they can read the message
            waitEnter();
            // Exit the function completely – nothing more to do
            return;
        }

        // Draw a horizontal line inside the box to separate sections
        printBoxSeparator();

        // Loop through every incoming friend request and display it with a number
        for (size_t i = 0; i < incoming.size(); ++i)
        {
            // Create a string like: "1. @john_doe"
            string line = to_string(i + 1) + ". @" + incoming[i];
            // Print this line left-aligned with proper padding inside the box
            printPaddedLine(line);
        }

        // Another separator line before the input prompt
        printBoxSeparator();
        
        // Prompt the user to enter which request they want to handle
        cout << "║ Enter Request Number (1 - " << incoming.size()
             << ") to Deal With OR Press 0 to go Back: ";
        
        int reqNum;  // Variable to store the number entered by the user
        if (!(cin >> reqNum)) {
            // invalid input
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            clearScreen();
            printBoxHeader("Invalid Input");
            printCenteredLine("Please Enter A Valid Number");
            printBoxFooter();
            waitEnter();
            continue;
        }
        // Clear the input buffer (removes leftover newline)
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // If user enters 0, go back to the previous menu
        if (reqNum == 0)
        {
            printBoxFooter();  // Close the box properly
            waitEnter();       // Let user see the screen before returning
            return;            // Exit the function
        }

        // Validate: user must enter a number between 1 and the total number of requests
        if (reqNum < 1 || reqNum > static_cast<int>(incoming.size()))
        {
            clearScreen();  // Start fresh for error message
            printBoxHeader("Invalid Input | Out of Range");
            printCenteredLine("Please Enter a Valid Request Number in Mentioned Range");
            printBoxFooter();
            waitEnter();
            continue;  // Go back to the top of the loop and show the list again
        }

        int idx = reqNum - 1;

        string decision;  // Will store Y or N (case-insensitive)
        while (true)  // Inner loop: keep asking until user types Y or N
        {
            clearScreen();  // Fresh screen for decision
            printBoxHeader("Friend Request from @" + incoming[idx]);
            printBoxSeparator();
            printCenteredLine("Do you want to Accept this request? (Y -> YES | N -> NO): ");
            
            getline(cin, decision);  // Read full line

            // ACCEPT THE REQUEST (Y or y)
            if (decision == "Y" || decision == "y")
            {
                string requesterName = incoming[idx];
                bool ok = false;
                if (UserHelper::friend_graph != nullptr) {
                    ok = UserHelper::friend_graph->acceptFriendRequest(requesterName, currentUser.getUsername());
                    if (ok) {
                        FileManager fman;
                        fman.saveFriendGraphToFile(*UserHelper::friend_graph);
                    }
                }

                // Show result
                clearScreen();
                if (ok) {
                    printBoxHeader("Friend Request Accepted");
                    printCenteredLine("Now You are Friends with @" + requesterName);
                } else {
                    printBoxHeader("Accept Failed");
                    printCenteredLine("Could not accept the request.");
                }
                printBoxFooter();
                waitEnter();
                break;  // Exit decision loop → go back to request list
            }

            // DECLINE THE REQUEST (N or n)
            else if (decision == "N" || decision == "n")
            {
                string requesterName = incoming[idx];
                bool ok = false;
                if (UserHelper::friend_graph != nullptr) {
                    ok = UserHelper::friend_graph->removeFriendRequest(requesterName, currentUser.getUsername());
                    if (ok) {
                        FileManager fman;
                        fman.saveFriendGraphToFile(*UserHelper::friend_graph);
                    }
                }

                clearScreen();
                if (ok) {
                    printBoxHeader("Friend Request Declined");
                    printCenteredLine("Request from @" + requesterName + " was declined");
                } else {
                    printBoxHeader("Decline Failed");
                    printCenteredLine("Could not decline the request.");
                }
                printBoxFooter();
                waitEnter();
                break;  // Back to list
            }
            // If anything else is typed -> loop again and ask for Y/N
        }
    }
}

void viewMyFriends(User& currentUser) // Receives the logged-in user by reference
{
    while (true) // Loop until user chooses to go back
    {
        clearScreen(); // Clear screen for clean display
        printBoxHeader("My Friends – @" + currentUser.getUsername()); // Show title with current username

        vector<string> friends;
        if (UserHelper::friend_graph != nullptr) {
            friends = UserHelper::friend_graph->getFriends(currentUser.getUsername());
        }

        if (friends.empty()) // Check if user has no friends
        {
            printCenteredLine("You Have No Friends Yet"); // Display message when no friends exist
            printBoxFooter(); // Close the UI box
            waitEnter(); // Wait for user to press Enter
            return; // Exit the function entirely
        }

        printBoxSeparator(); // Draw a separator line

        // Loop through all friends and display them with numbering
        for (size_t i = 0; i < friends.size(); ++i)
        {
            string line = to_string(i + 1) + ". @" + friends[i]; // Format: "1. @username"
            printPaddedLine(line); // Print each friend
        }

        printBoxSeparator(); // Another separator before prompt

        cout << "║ Want to Visit any of Them ? (1 -> Yes | 0 -> No -> Go Back): "; // Ask if user wants to view a friend's profile
        string decide;
        getline(cin, decide); // Read user's decision

        if (decide == "1") // User wants to visit a friend
        {
            cout << "║ Enter Friend Number (1-" << friends.size() << "): "; // Prompt for friend number
            string numStr;
            getline(cin, numStr); // Read input as string

            int friendNum;
            try {
                friendNum = stoi(numStr); // Convert input to integer
            }
            catch (...) { // Handle invalid number input
                clearScreen(); // Clear screen for error
                printBoxHeader("Invalid Input"); // Show error title
                printCenteredLine("Please Enter A Valid Number"); // Error message
                printBoxFooter(); // Close box
                waitEnter(); // Pause
                continue; // Restart loop
            }

            // Validate if entered number is within valid range
            if (friendNum < 1 || friendNum > static_cast<int>(friends.size()))
            {
                clearScreen(); // Clear screen
                printBoxHeader("Invalid Number"); // Error title
                printCenteredLine("Number Out Of Range"); // Error message
                printBoxFooter(); // Close box
                waitEnter(); // Pause
                continue; // Restart loop
            }

            string targetUsername = friends[friendNum - 1]; // Get selected friend's username (1-based to 0-based index)
            FileManager fman;
            User targetUser = fman.getUserByUsername(targetUsername);

            if (targetUser.getUsername().empty()) // Rare case: not found
            {
                clearScreen(); // Clear screen
                printBoxHeader("Error"); // Error title
                printCenteredLine("User Not Found In System"); // Error message
                printBoxFooter(); // Close box
                waitEnter(); // Pause
            }
            else
            {
                viewOtherProfile(currentUser, targetUser);
            }
        }
        else if (decide == "0") // User wants to go back
        {
            printBoxFooter(); // Close the box properly
            return; // Exit function and return to previous menu
        }
        else // Any other input is invalid
        {
            clearScreen(); // Clear screen
            printBoxHeader("Invalid Choice"); // Error title
            printCenteredLine("Please Enter 1 or 0"); // Error message
            printBoxFooter(); // Pause
            waitEnter(); // Pause
        }
    }
}

void viewMutualFriends(User& currentUser, User& otherUser) {
	while(true){
    	clearScreen();
    	printBoxHeader("Mutual Friends with @" + otherUser.getUsername());

    	vector<string> mutual;
    	if (UserHelper::friend_graph != nullptr) {
        	mutual = UserHelper::friend_graph->getMutualFriends(currentUser.getUsername(), otherUser.getUsername());
    	}

    	if (mutual.empty()) {
        	printCenteredLine("No Mutual Friends");
        	printBoxFooter();
        	waitEnter();
        	return;
    	}

    	printBoxSeparator();
    	for (size_t i = 0; i < mutual.size(); ++i) {
        	printPaddedLine(to_string(i + 1) + ". @" + mutual[i]);
    	}
    	printBoxSeparator();
    	cout << "║ Want to Visit any of Them ? (1 -> Yes | 0 -> No -> Go Back): ";
    	string decide;
    	getline(cin, decide);
    	if (decide == "1") // User wants to visit a friend
        {
            cout << "║ Enter Friend Number (1-" << mutual.size() << "): "; // Prompt for friend number
            string numStr;
            getline(cin, numStr); // Read input as string

            int friendNum;
            try {
                friendNum = stoi(numStr); // Convert input to integer
            }
            catch (...) { // Handle invalid number input
                clearScreen(); // Clear screen for error
                printBoxHeader("Invalid Input"); // Show error title
                printCenteredLine("Please Enter A Valid Number"); // Error message
                printBoxFooter(); // Close box
                waitEnter(); // Pause
                continue; // Restart loop
            }

            // Validate if entered number is within valid range
            if (friendNum < 1 || friendNum > static_cast<int>(mutual.size()))
            {
                clearScreen(); // Clear screen
                printBoxHeader("Invalid Number"); // Error title
                printCenteredLine("Number Out Of Range"); // Error message
                printBoxFooter(); // Close box
                waitEnter(); // Pause
                continue; // Restart loop
            }

            string targetUsername = mutual[friendNum - 1]; // Get selected friend's username (1-based to 0-based index)
            FileManager fman;
            User targetUser = fman.getUserByUsername(targetUsername);

            if (targetUser.getUsername().empty()) // Rare case: not found
            {
                clearScreen(); // Clear screen
                printBoxHeader("Error"); // Error title
                printCenteredLine("User Not Found In System"); // Error message
                printBoxFooter(); // Close box
                waitEnter(); // Pause
            }
            else
            {
                viewOtherProfile(currentUser, targetUser);
            }
        }
        else if (decide == "0") // User wants to go back
        {
            printBoxFooter(); // Close the box properly
            return; // Exit function and return to previous menu
        }
        else // Any other input is invalid
        {
            clearScreen(); // Clear screen
            printBoxHeader("Invalid Choice"); // Error title
            printCenteredLine("Please Enter 1 or 0"); // Error message
            printBoxFooter(); // Pause
            waitEnter(); // Pause
        }
    }
}

void showFriendSuggestions(User& currentUser) {
    while (true)
    {
        clearScreen();
        printBoxHeader("Suggested Friends for @" + currentUser.getUsername());

        // Fetch prepared suggestions (username, reason)
        auto suggestions = getSuggestedFriendsWithReasons(
            currentUser.getUsername(),
            UserHelper::all_usernames,
            UserHelper::friend_graph,
            5
        );

        if (UserHelper::suggestion_graph == nullptr) {
            printCenteredLine("Suggestion graph not initialized.");
            printCenteredLine("Please view your profile first.");
            printBoxFooter();
            waitEnter();
            return;
        }

        if (suggestions.empty()) {
            printCenteredLine("No Suggested Users Available Right Now");
            printBoxFooter();
            waitEnter();
            return;
        }

        printBoxSeparator();

        // Display suggestions with reasons
        for (size_t i = 0; i < suggestions.size(); ++i)
        {
            string line = to_string(i + 1) + ". @" + suggestions[i].first;
            line += "  (Reason: " + suggestions[i].second + ")";
            printPaddedLine(line);
        }

        printBoxSeparator();

        cout << "║ Want to Visit any of Them ? (1 -> Yes | 0 -> No -> Go Back): ";
        string decide;
        getline(cin, decide);

        if (decide == "1")
        {
            cout << "║ Enter Suggestion Number (1-" << suggestions.size() << "): ";
            string numStr;
            getline(cin, numStr);

            int selectedNum;
            try {
                selectedNum = stoi(numStr);
            }
            catch (...) {
                clearScreen();
                printBoxHeader("Invalid Input");
                printCenteredLine("Please Enter A Valid Number");
                printBoxFooter();
                waitEnter();
                continue;
            }

            if (selectedNum < 1 || selectedNum > static_cast<int>(suggestions.size()))
            {
                clearScreen();
                printBoxHeader("Invalid Number");
                printCenteredLine("Number Out Of Range");
                printBoxFooter();
                waitEnter();
                continue;
            }

            string targetUsername = suggestions[selectedNum - 1].first;

            FileManager fman;
            User targetUser = fman.getUserByUsername(targetUsername);

            if (targetUser.getUsername().empty()) {
                clearScreen();
                printBoxHeader("Error");
                printCenteredLine("User Not Found In System");
                printBoxFooter();
                waitEnter();
            }
            else {
                // Visit profile
                viewOtherProfile(currentUser, targetUser);
            }
        }
        else if (decide == "0")
        {
            printBoxFooter();
            return;
        }
        else
        {
            clearScreen();
            printBoxHeader("Invalid Choice");
            printCenteredLine("Please Enter 1 or 0");
            printBoxFooter();
            waitEnter();
        }
    }
}

void viewOtherProfile(User& currentUser, User& otherUser) {
    while (true) {
        clearScreen();
        printBoxHeader("Viewing Profile Of | @" + otherUser.getUsername());
        
        // Show full name if set
        if (!otherUser.getName().empty()) {
            printCenteredLine(otherUser.getName());
            printBoxSeparator();
        } else {
            printCenteredLine("No Name Set Yet by @" + otherUser.getUsername());
            printBoxSeparator();
        }
        
        printPaddedLine("Email: ", otherUser.getEmail());
        
        // Show optional fields only if they are not empty
        if (!otherUser.getGender().empty()) {
            printPaddedLine("Gender: ", otherUser.getGender());
        }
        if (!otherUser.getAge().empty()) {
            printPaddedLine("Age: ", otherUser.getAge());
        }
        if (!otherUser.getOccupation().empty()) {
            printPaddedLine("Occupation: ", otherUser.getOccupation());
        }
        if (!otherUser.getBio().empty()) {
            printPaddedLine("Bio: ", otherUser.getBio());
        }
        if (!otherUser.getCity().empty()) {
            printPaddedLine("City: ", otherUser.getCity());
        }
        if (!otherUser.getCountry().empty()) {
            printPaddedLine("Country: ", otherUser.getCountry());
        }
        
        // Show birth date only if it's not the default value
        Date birthDate = otherUser.getBirthDate();
        if (!(birthDate.getDay() == 0 && birthDate.getMonth() == 0 && birthDate.getYear() == 0)) {
            printPaddedLine("Birth Date: ", birthDate.toString());
        }
        
        printBoxSeparator();
        printPaddedLine("Posts: ", to_string(otherUser.getRecentPosts().size()));
        // Friends count from graph
        if (UserHelper::friend_graph != nullptr) {
            auto otherFriends = UserHelper::friend_graph->getFriends(otherUser.getUsername());
            printPaddedLine("Friends: ", to_string(otherFriends.size()));
        } else {
            printPaddedLine("Friends: ", "0");
        }
        printBoxSeparator();
        printDynamicGrid(otherUser);
        printBoxSeparator();
        
        // Mutual friends (show count only if > 0)
        if (UserHelper::friend_graph != nullptr) {
            auto mutual = UserHelper::friend_graph->getMutualFriends(currentUser.getUsername(), otherUser.getUsername());
            if (!mutual.empty()) {
                printPaddedLine("Mutual Friends: ", to_string(mutual.size()));
                printBoxSeparator();
            }
        }

        // Determine friend button text based on current relationship
        string buttonText;
        if (currentUser.isFriend(otherUser.getUsername())) {
            buttonText = "Already Friends"; // Already friends
        }
        else if (currentUser.hasSentRequest(otherUser.getUsername())) {
            buttonText = "Friend Request Pending"; // Request sent, awaiting response
        }
        else if (currentUser.hasIncomingRequest(otherUser.getUsername())) {
            buttonText = "Accept Friend Request"; // They sent request to you
        }
        else {
            buttonText = "Send Friend Request"; // No connection yet
        }

        // Action menu
        printPaddedLine("1. " + buttonText);
        printPaddedLine("2. View Mutual Friends");
        printPaddedLine("3. View Posts");
        printPaddedLine("0. Back");
        printBoxFooter();
        
        string choice;
        cout << "\nChoice: ";
        getline(cin, choice);
        
        if (choice == "1") {
            // Handle friend action based on current state
            if (UserHelper::friend_graph == nullptr) {
                clearScreen();
                printBoxHeader("Error");
                printCenteredLine("Friend graph not initialized.");
                printBoxFooter();
                waitEnter();
            }
            else if (buttonText == "Send Friend Request") {
                bool ok = UserHelper::friend_graph->sendFriendRequest(currentUser.getUsername(), otherUser.getUsername());
                if (ok) {
                    FileManager fman;
                    fman.saveFriendGraphToFile(*UserHelper::friend_graph);
                    clearScreen();
                    printBoxHeader("Friend Request Sent");
                    printCenteredLine("Request sent to: " + otherUser.getUsername());
                    printBoxFooter();
                    waitEnter();
                } else {
                    clearScreen();
                    printBoxHeader("Request Failed");
                    printCenteredLine("Could not send friend request. It may already exist or you are friends.");
                    printBoxFooter();
                    waitEnter();
                }
            }
            else if (buttonText == "Accept Friend Request") {
                // otherUser sent request to currentUser --> accept
                bool ok = UserHelper::friend_graph->acceptFriendRequest(otherUser.getUsername(), currentUser.getUsername());
                if (ok) {
                    FileManager fman;
                    fman.saveFriendGraphToFile(*UserHelper::friend_graph);
                    clearScreen();
                    printBoxHeader("Friend Request Accepted");
                    printCenteredLine("You are now friends with: " + otherUser.getUsername());
                    printBoxFooter();
                    waitEnter();
                } else {
                    clearScreen();
                    printBoxHeader("Accept Failed");
                    printCenteredLine("Could not accept friend request.");
                    printBoxFooter();
                    waitEnter();
                }
            }
            else if (buttonText == "Friend Request Pending") {
                clearScreen();
                printBoxHeader("Pending");
                printCenteredLine("Friend request is pending.");
                printBoxFooter();
                waitEnter();
            }
            else if (buttonText == "Already Friends") {
                clearScreen();
                printBoxHeader("Info");
                printCenteredLine("You are already friends with: " + otherUser.getUsername());
                printBoxFooter();
                waitEnter();
            }
        }
        else if (choice == "2") {
            // View mutual friends between currentUser and otherUser
            viewMutualFriends(currentUser, otherUser);
        }
        else if (choice == "3") {
            viewInterests(otherUser);
        }
        else if (choice == "0") {
            break;
        }
    }
}

