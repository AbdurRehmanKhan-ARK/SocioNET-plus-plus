#include<iostream>
// user.cpp
// Implementations for User class declared in include/User.h

#include "../include/User.h"
#include "../include/Graph.h"

User::User() : username(""), password(""), Name(""), email(""), birth_date() {}

User::User(const std::string &uname) : username(uname), password(""), Name(""), email(""), birth_date() {}

// Helper lambdas for string and vector I/O kept local to this translation unit
namespace {
	inline void writeString(std::ostream &os, const std::string &s) {
		size_t len = s.size();
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		os.write(s.data(), static_cast<std::streamsize>(len));
	}
	inline void readString(std::istream &is, std::string &s) {
		size_t len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		s.resize(len);
		if (len) is.read(&s[0], static_cast<std::streamsize>(len));
	}

	template <typename T>
	inline void writeVector(std::ostream &os, const std::vector<T> &vec) {
		size_t n = vec.size();
		os.write(reinterpret_cast<const char*>(&n), sizeof(n));
		for (const auto &item : vec) {
			item.serialize(os);
		}
	}

	template <typename T>
	inline void readVector(std::istream &is, std::vector<T> &vec) {
		size_t n = 0;
		is.read(reinterpret_cast<char*>(&n), sizeof(n));
		vec.clear();
		vec.resize(n);
		for (size_t i = 0; i < n; ++i) {
			vec[i].deserialize(is);
		}
	}
}

void User::serialize(std::ostream &os) const {
	// Primitive and string fields
	writeString(os, username);
	writeString(os, password);
	writeString(os, Name);
	writeString(os, email);
	writeString(os, gender);
	writeString(os, age);
	writeString(os, occupation);
	writeString(os, bio);
	writeString(os, city);
	writeString(os, country);

	// Date
	int d = birth_date.getDay();
	int m = birth_date.getMonth();
	int y = birth_date.getYear();
	os.write(reinterpret_cast<const char*>(&d), sizeof(d));
	os.write(reinterpret_cast<const char*>(&m), sizeof(m));
	os.write(reinterpret_cast<const char*>(&y), sizeof(y));

	// Favorite single interests
	favorite_book.serialize(os);
	favorite_movie.serialize(os);
	favorite_place.serialize(os);
	favorite_food.serialize(os);
	favorite_sport.serialize(os);

	// Vectors of interests
	writeVector(os, read_books);
	writeVector(os, travelled_places);
	writeVector(os, tried_foods);
	writeVector(os, attended_sports_events);
	writeVector(os, watched_movies);

	// Recent posts stack
	recent_posts.serialize(os);
}

// ============================================================================
// Friend relationship helper implementations
// ============================================================================

bool User::isFriend(const std::string& other) const {
    if (UserHelper::friend_graph == nullptr) return false;
    return UserHelper::friend_graph->areFriends(this->username, other);
}

bool User::hasSentRequest(const std::string& other) const {
    if (UserHelper::friend_graph == nullptr) return false;
    return UserHelper::friend_graph->hasSentRequest(this->username, other);
}

bool User::hasIncomingRequest(const std::string& other) const {
    if (UserHelper::friend_graph == nullptr) return false;
    return UserHelper::friend_graph->hasIncomingRequest(this->username, other);
}

void User::deserialize(std::istream &is) {
	// Primitive and string fields
	readString(is, username);
	readString(is, password);
	readString(is, Name);
	readString(is, email);
	readString(is, gender);
	readString(is, age);
	readString(is, occupation);
	readString(is, bio);
	readString(is, city);
	readString(is, country);

	// Date
	int d=0,m=0,y=0;
	is.read(reinterpret_cast<char*>(&d), sizeof(d));
	is.read(reinterpret_cast<char*>(&m), sizeof(m));
	is.read(reinterpret_cast<char*>(&y), sizeof(y));
	birth_date.setDay(d);
	birth_date.setMonth(m);
	birth_date.setYear(y);

	// Favorite single interests
	favorite_book.deserialize(is);
	favorite_movie.deserialize(is);
	favorite_place.deserialize(is);
	favorite_food.deserialize(is);
	favorite_sport.deserialize(is);

	// Vectors of interests
	readVector(is, read_books);
	readVector(is, travelled_places);
	readVector(is, tried_foods);
	readVector(is, attended_sports_events);
	readVector(is, watched_movies);

	// Recent posts stack
	recent_posts.deserialize(is);
}