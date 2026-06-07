// User.h
// Declaration of User class. This header shows how to include Interest and Helper headers
// so a User can contain interest objects and Date objects.

#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "Helper.h"
#include "Interest.h"
using std::string;
using std::vector;
using std::ostream;
using std::istream;
using std::cout;

// User: represents a simple user in the social graph.
// Add/remove members as your project needs; this file contains examples of including
// interest types and using the Date utility from Helper.h.
class User {
private:
    string username; // Main identifier (No duplication allowed here between users)
    // TODO: Store password securely (hashed + salted) in final code
    string password; // In production, store a hash, not plaintext
    string Name; // Displayed as secondary when profile is viewed
    string email; // For contact, password recovery, etc.
    // Secondary user information
    string gender;
    string age;
    string occupation;
    string bio; // Short description
    string city;
    string country;

    // Example birth date using the Date type defined in Helper.h
    Date birth_date;

    // Example interests: keep them as single objects here; later you may want vectors
    BookInterest favorite_book;
    MovieInterest favorite_movie;
    PlaceInterest favorite_place;
    FoodInterest favorite_food;
    SportInterest favorite_sport;

    // Vectors of interests
    vector<BookInterest> read_books;
    vector<PlaceInterest> travelled_places;
    vector<FoodInterest> tried_foods;
    vector<SportInterest> attended_sports_events;
    vector<MovieInterest> watched_movies;

    // Vector for recent posts of user using InterestVariant defined in Interest.h
    InterestStack recent_posts;

public:
    User();
    User(const string &uname);

    // Setters/getters
    void setUsername(const string &u) { username = u; }
    string getUsername() const { return username; }
    
    void setPassword(const string &p) { password = p; }
    string getPassword() const { return password; }
    
    void setName(const string &n) { Name = n; }
    string getName() const { return Name; }
    
    void setEmail(const string &e) { email = e; }
    string getEmail() const { return email; }

    void setBirthDate(const Date &d) { birth_date = d; }
    Date getBirthDate() const { return birth_date; }
    
    // Profile field getters/setters for UI
    void setGender(const string &g) { gender = g; }
    string getGender() const { return gender; }
    
    void setAge(const string &a) { age = a; }
    string getAge() const { return age; }
    
    void setOccupation(const string &o) { occupation = o; }
    string getOccupation() const { return occupation; }
    
    void setBio(const string &b) { bio = b; }
    string getBio() const { return bio; }
    
    void setCity(const string &c) { city = c; }
    string getCity() const { return city; }
    
    void setCountry(const string &c) { country = c; }
    string getCountry() const { return country; }
    
    // Interest access methods for UI
    const InterestStack& getRecentPosts() const { return recent_posts; }
    InterestStack& getRecentPosts() { return recent_posts; }
    
    const BookInterest& getFavoriteBook() const { return favorite_book; }
    BookInterest& getFavoriteBook() { return favorite_book; }
    
    const MovieInterest& getFavoriteMovie() const { return favorite_movie; }
    MovieInterest& getFavoriteMovie() { return favorite_movie; }
    
    const PlaceInterest& getFavoritePlace() const { return favorite_place; }
    PlaceInterest& getFavoritePlace() { return favorite_place; }
    
    const FoodInterest& getFavoriteFood() const { return favorite_food; }
    FoodInterest& getFavoriteFood() { return favorite_food; }
    
    const SportInterest& getFavoriteSport() const { return favorite_sport; }
    SportInterest& getFavoriteSport() { return favorite_sport; }
    
    // Get interest vectors for UI
    const vector<BookInterest>& getReadBooks() const { return read_books; }
    vector<BookInterest>& getReadBooks() { return read_books; }
    
    const vector<MovieInterest>& getWatchedMovies() const { return watched_movies; }
    vector<MovieInterest>& getWatchedMovies() { return watched_movies; }
    
    const vector<PlaceInterest>& getTravelledPlaces() const { return travelled_places; }
    vector<PlaceInterest>& getTravelledPlaces() { return travelled_places; }
    
    const vector<FoodInterest>& getTriedFoods() const { return tried_foods; }
    vector<FoodInterest>& getTriedFoods() { return tried_foods; }
    
    const vector<SportInterest>& getAttendedSportsEvents() const { return attended_sports_events; }
    vector<SportInterest>& getAttendedSportsEvents() { return attended_sports_events; }
    
    // Helper method to calculate profile completion percentage
    double profileCompletion() const {
        int score = 0;
        if (!Name.empty()) score++;
        if (!gender.empty()) score++;
        if (!age.empty()) score++;
        if (!occupation.empty()) score++;
        if (!bio.empty()) score++;
        if (!city.empty()) score++;
        if (!country.empty()) score++;
        if (birth_date.getDay() != 0 || birth_date.getMonth() != 0 || birth_date.getYear() != 0) score++;
        return (score / 8.0) * 100.0;
    }

    // Friend relationship helpers (delegates to global graph)
    bool isFriend(const std::string& username) const;
    bool hasSentRequest(const std::string& username) const;
    bool hasIncomingRequest(const std::string& username) const;

    // Binary serialization/deserialization for the entire User object
    void serialize(ostream &os) const;
    void deserialize(istream &is);
};

#endif // USER_H
