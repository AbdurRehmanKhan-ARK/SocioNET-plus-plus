// Interest.h
// Declarations for various interest types used by User objects.
// Each interest class is lightweight; expand as needed.

#ifndef INTEREST_H
#define INTEREST_H

#include <string>
#include <vector>
#include <variant> // for std::variant
#include <istream>
#include <ostream>
#include <cstdint>
#include "Helper.h" // for Date if needed by some interest types
using std::string;
using std::variant;
using std::istream;
using std::ostream;
using std::vector;
using std::fstream;

// BookInterest: stores basic information about a user's book preference
class BookInterest {
private:
    string book_title;
    string author;
    string genre; // e.g., Fiction, Non-Fiction, Mystery, Sci-Fi, etc.
    int publication_year{0};
    string language;
    string publisher;
public:
    BookInterest() = default;
    BookInterest(const string &title, const string &auth);
    // Getters
    string getTitle() const { return book_title; }
    string getAuthor() const { return author; }
    string getGenre() const { return genre; }
    int getYear() const { return publication_year; }
    string getLanguage() const { return language; }
    string getPublisher() const { return publisher; }

    // Setters
    void setTitle(const string &t) { book_title = t; }
    void setAuthor(const string &a) { author = a; }
    void setGenre(const string &g) { genre = g; }
    void setYear(int y) { publication_year = y; }
    void setLanguage(const string &l) { language = l; }
    void setPublisher(const string &p) { publisher = p; }

    // Binary serialization helpers
    void serialize(ostream &os) const;
    void deserialize(istream &is);
};

// MovieInterest: stores basic information about a user's movie preference
class MovieInterest {
private:
    string movie_title;
    string director;
    string genre;
    int release_year{0};
    string language;
    string production_company;
public:
    MovieInterest() = default;
    MovieInterest(const string &title, const string &dir);

    // Getters
    string getTitle() const { return movie_title; }
    string getDirector() const { return director; }
    string getGenre() const { return genre; }
    int getYear() const { return release_year; }
    string getLanguage() const { return language; }
    string getProduction() const { return production_company; }

    // Setters
    void setTitle(const string &t) { movie_title = t; }
    void setDirector(const string &d) { director = d; }
    void setGenre(const string &g) { genre = g; }
    void setYear(int y) { release_year = y; }
    void setLanguage(const string &l) { language = l; }
    void setProduction(const string &p) { production_company = p; }

    void serialize(ostream &os) const;
    void deserialize(istream &is);
};

// PlaceInterest: stores a preferred place or travel-related interest
class PlaceInterest {
private:
    string place_name;
    string country;
    string city;
    string type; // e.g., Historical, Natural, Urban
    Date visit_date;
    string best_season_to_visit;
    string activities_available;
public:
    PlaceInterest() = default;
    PlaceInterest(const string &name);

    // Getters
    string getName() const { return place_name; }
    string getCountry() const { return country; }
    string getCity() const { return city; }
    string getType() const { return type; }
    Date getVisitDate() const { return visit_date; }
    string getBestSeason() const { return best_season_to_visit; }
    string getActivities() const { return activities_available; }

    // Setters
    void setName(const string &n) { place_name = n; }
    void setCountry(const string &c) { country = c; }
    void setCity(const string &c) { city = c; }
    void setType(const string &t) { type = t; }
    void setVisitDate(const Date &d) { visit_date = d; }
    void setBestSeason(const string &s) { best_season_to_visit = s; }
    void setActivities(const string &a) { activities_available = a; }

    void serialize(ostream &os) const;
    void deserialize(istream &is);
};

// FoodInterest: stores information about food/dish interests
class FoodInterest {
private:
    string dish_name;
    string cuisine;
    string dining_place; // Home, Restaurant, Cafe, etc.
    string main_ingredients;
    string meal_type; // Main course, dessert, dinner, etc.
public:
    FoodInterest() = default;
    FoodInterest(const string &dish);

    // Getters
    string getDish() const { return dish_name; }
    string getCuisine() const { return cuisine; }
    string getPlace() const { return dining_place; }
    string getIngredients() const { return main_ingredients; }
    string getMeal() const { return meal_type; }

    // Setters
    void setDish(const string &d) { dish_name = d; }
    void setCuisine(const string &c) { cuisine = c; }
    void setPlace(const string &p) { dining_place = p; }
    void setIngredients(const string &i) { main_ingredients = i; }
    void setMeal(const string &m) { meal_type = m; }

    void serialize(ostream &os) const;
    void deserialize(istream &is);
};

// SportInterest: stores information about sports preferences
class SportInterest {
private:
    string sport_name;
    string league_name;
    string team_one_name;
    string team_two_name;
    Date event_date; // uses Date from Helper.h
    string venue;
public:
    SportInterest() = default;
    SportInterest(const string &sport, const Date &d);

    // Getters
    string getSport() const { return sport_name; }
    string getLeague() const { return league_name; }
    string getTeam1() const { return team_one_name; }
    string getTeam2() const { return team_two_name; }
    Date getEventDate() const { return event_date; }
    string getVenue() const { return venue; }

    // Setters
    void setSport(const string &s) { sport_name = s; }
    void setLeague(const string &l) { league_name = l; }
    void setTeam1(const string &t) { team_one_name = t; }
    void setTeam2(const string &t) { team_two_name = t; }
    void setEventDate(const Date &d) { event_date = d; }
    void setVenue(const string &v) { venue = v; }

    void serialize(ostream &os) const;
    void deserialize(istream &is);
};

// Very important concept here of std::variant or union to store different interest (data) types in a single stack
// 'using' is a type alias. that is: using = typedef
using InterestVariant = variant<BookInterest, MovieInterest, PlaceInterest, FoodInterest, SportInterest>;

// This class will act as a recent post stack which will be showed when the user profile is viewed
class InterestStack{
private:
    vector<InterestVariant> stack;
public:
    void push(const InterestVariant& interest);
    void pop();
    const vector<InterestVariant>& getStack() const;
    InterestVariant* top();
    bool empty() const;
    size_t size() const;
    void printTop() const;

    // Binary serialization for the stack of variants
    void serialize(ostream &os) const;
    void deserialize(istream &is);
};

#endif // INTEREST_H