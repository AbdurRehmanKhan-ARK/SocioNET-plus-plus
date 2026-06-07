#include<iostream>
// interest.cpp
// Implementations for interest classes declared in include/Interest.h
#include "../include/Interest.h"
// Simple constructors - keep them minimal. Expand as needed.
BookInterest::BookInterest(const std::string &title, const std::string &auth)
    : book_title(title), author(auth) {}

// Serialize/Deserialize for BookInterest
void BookInterest::serialize(ostream &os) const {
	auto writeString = [&](const string &s) {
		size_t len = s.size();
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		os.write(s.data(), static_cast<std::streamsize>(len));
	};
	writeString(book_title);
	writeString(author);
	writeString(genre);
	os.write(reinterpret_cast<const char*>(&publication_year), sizeof(publication_year));
	writeString(language);
	writeString(publisher);
}
void BookInterest::deserialize(istream &is) {
	auto readString = [&](string &s) {
		size_t len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		s.resize(len);
		if (len) is.read(&s[0], static_cast<std::streamsize>(len));
	};
	readString(book_title);
	readString(author);
	readString(genre);
	is.read(reinterpret_cast<char*>(&publication_year), sizeof(publication_year));
	readString(language);
	readString(publisher);
}

// Serialize/Deserialize for MovieInterest
MovieInterest::MovieInterest(const std::string &title, const std::string &dir)
    : movie_title(title), director(dir) {}

void MovieInterest::serialize(ostream &os) const {
	auto writeString = [&](const string &s) {
		size_t len = s.size();
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		os.write(s.data(), static_cast<std::streamsize>(len));
	};
	writeString(movie_title);
	writeString(director);
	writeString(genre);
	os.write(reinterpret_cast<const char*>(&release_year), sizeof(release_year));
	writeString(language);
	writeString(production_company);
}
void MovieInterest::deserialize(istream &is) {
	auto readString = [&](string &s) {
		size_t len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		s.resize(len);
		if (len) is.read(&s[0], static_cast<std::streamsize>(len));
	};
	readString(movie_title);
	readString(director);
	readString(genre);
	is.read(reinterpret_cast<char*>(&release_year), sizeof(release_year));
	readString(language);
	readString(production_company);
}

// Serialize/Deserialize for PlaceInterest
PlaceInterest::PlaceInterest(const std::string &name)
    : place_name(name) {}

void PlaceInterest::serialize(ostream &os) const {
	auto writeString = [&](const string &s) {
		size_t len = s.size();
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		os.write(s.data(), static_cast<std::streamsize>(len));
	};
	writeString(place_name);
	writeString(country);
	writeString(city);
	writeString(type);
	// Date
	int d = visit_date.getDay();
	int m = visit_date.getMonth();
	int y = visit_date.getYear();
	os.write(reinterpret_cast<const char*>(&d), sizeof(d));
	os.write(reinterpret_cast<const char*>(&m), sizeof(m));
	os.write(reinterpret_cast<const char*>(&y), sizeof(y));
}
void PlaceInterest::deserialize(istream &is) {
	auto readString = [&](string &s) {
		size_t len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		s.resize(len);
		if (len) is.read(&s[0], static_cast<std::streamsize>(len));
	};
	readString(place_name);
	readString(country);
	readString(city);
	readString(type);
	int d=0,m=0,y=0;
	is.read(reinterpret_cast<char*>(&d), sizeof(d));
	is.read(reinterpret_cast<char*>(&m), sizeof(m));
	is.read(reinterpret_cast<char*>(&y), sizeof(y));
	visit_date.setDay(d);
	visit_date.setMonth(m);
	visit_date.setYear(y);
}

// Serialize/Deserialize for FoodInterest
FoodInterest::FoodInterest(const std::string &dish)
    : dish_name(dish) {}

void FoodInterest::serialize(ostream &os) const {
	auto writeString = [&](const string &s) {
		size_t len = s.size();
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		os.write(s.data(), static_cast<std::streamsize>(len));
	};
	writeString(dish_name);
	writeString(cuisine);
	writeString(dining_place);
	writeString(main_ingredients);
	writeString(meal_type);
}
void FoodInterest::deserialize(istream &is) {
	auto readString = [&](string &s) {
		size_t len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		s.resize(len);
		if (len) is.read(&s[0], static_cast<std::streamsize>(len));
	};
	readString(dish_name);
	readString(cuisine);
	readString(dining_place);
	readString(main_ingredients);
	readString(meal_type);
}

// Serialize/Deserialize for SportInterest
SportInterest::SportInterest(const std::string &sport, const Date &d)
    : sport_name(sport), event_date(d) {}

void SportInterest::serialize(ostream &os) const {
	auto writeString = [&](const string &s) {
		size_t len = s.size();
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		os.write(s.data(), static_cast<std::streamsize>(len));
	};
	writeString(sport_name);
	writeString(league_name);
	writeString(team_one_name);
	writeString(team_two_name);
	int d = event_date.getDay();
	int m = event_date.getMonth();
	int y = event_date.getYear();
	os.write(reinterpret_cast<const char*>(&d), sizeof(d));
	os.write(reinterpret_cast<const char*>(&m), sizeof(m));
	os.write(reinterpret_cast<const char*>(&y), sizeof(y));
	writeString(venue);
}
void SportInterest::deserialize(istream &is) {
	auto readString = [&](string &s) {
		size_t len = 0;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		s.resize(len);
		if (len) is.read(&s[0], static_cast<std::streamsize>(len));
	};
	readString(sport_name);
	readString(league_name);
	readString(team_one_name);
	readString(team_two_name);
	int d=0,m=0,y=0;
	is.read(reinterpret_cast<char*>(&d), sizeof(d));
	is.read(reinterpret_cast<char*>(&m), sizeof(m));
	is.read(reinterpret_cast<char*>(&y), sizeof(y));
	event_date.setDay(d);
	event_date.setMonth(m);
	event_date.setYear(y);
	readString(venue);
}


// INTERESTSTACK METHODS

void InterestStack::push(const InterestVariant& interest) {
        stack.push_back(interest);
    }

void InterestStack::pop() {
        if (!stack.empty()) {
            stack.pop_back();
        }
    }

const vector<InterestVariant>& InterestStack::getStack() const {
        return stack;
    }

InterestVariant* InterestStack::top() {
        if (stack.empty()) return nullptr;
        return &stack.back();
    }

bool InterestStack::empty() const {
        return stack.empty();
    }

size_t InterestStack::size() const {
        return stack.size();
    }


// InterestStack method to print the most recent post made by the user
void InterestStack::printTop() const {
    if (stack.empty()) {
        std::cout << "Stack is empty.\n";
        return;
    }

    // Another important concept: std::visit for variant types
    // This function uses std::visit to handle the variant types
    // The following lambda checks the type held by the variant and prints a message accordingly
    // The real logic will be implemented later on 
    std::visit([](const auto &interest) {
        using T = std::decay_t<decltype(interest)>;

        if constexpr (std::is_same_v<T, BookInterest>)
            std::cout << "Top Interest: A Book\n";
        else if constexpr (std::is_same_v<T, MovieInterest>)
            std::cout << "Top Interest: A Movie\n";
        else if constexpr (std::is_same_v<T, PlaceInterest>)
            std::cout << "Top Interest: A Place\n";
        else if constexpr (std::is_same_v<T, FoodInterest>)
            std::cout << "Top Interest: A Food\n";
        else if constexpr (std::is_same_v<T, SportInterest>)
            std::cout << "Top Interest: A Sport\n";
    }, stack.back());
}

// Binary serialization for InterestStack
void InterestStack::serialize(ostream &os) const {
	// Write size
	size_t n = stack.size();
	os.write(reinterpret_cast<const char*>(&n), sizeof(n));
	// For each element, write a type tag then payload
	for (const auto &v : stack) {
		std::visit([&](const auto &val) {
			using T = std::decay_t<decltype(val)>;
			uint8_t tag = 0;
			if constexpr (std::is_same_v<T, BookInterest>) tag = 0;
			else if constexpr (std::is_same_v<T, MovieInterest>) tag = 1;
			else if constexpr (std::is_same_v<T, PlaceInterest>) tag = 2;
			else if constexpr (std::is_same_v<T, FoodInterest>) tag = 3;
			else if constexpr (std::is_same_v<T, SportInterest>) tag = 4;
			os.write(reinterpret_cast<const char*>(&tag), sizeof(tag));
			val.serialize(os);
		}, v);
	}
}

void InterestStack::deserialize(istream &is) {
	// Read size
	size_t n = 0;
	is.read(reinterpret_cast<char*>(&n), sizeof(n));
	stack.clear();
	stack.reserve(n);
	for (size_t i = 0; i < n; ++i) {
		uint8_t tag = 0;
		is.read(reinterpret_cast<char*>(&tag), sizeof(tag));
		switch (tag) {
			case 0: {
				BookInterest b; b.deserialize(is); stack.emplace_back(b); break;
			}
			case 1: {
				MovieInterest m; m.deserialize(is); stack.emplace_back(m); break;
			}
			case 2: {
				PlaceInterest p; p.deserialize(is); stack.emplace_back(p); break;
			}
			case 3: {
				FoodInterest f; f.deserialize(is); stack.emplace_back(f); break;
			}
			case 4: {
				SportInterest s; s.deserialize(is); stack.emplace_back(s); break;
			}
			default: {
				// Unknown tag; abort safely by clearing remaining
				return;
			}
		}
	}
}