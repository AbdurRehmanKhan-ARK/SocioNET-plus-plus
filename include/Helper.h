// Helper.h
// Small utility types used across the Social-Networking-Graph project.
// This header declares the Date type used by interest and user classes.

#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <vector>
#include <functional> // for std::hash
#include <unordered_map>

// Forward declaration for Graph (to avoid circular includes)
class Graph;
class SuggestionGraph;

// ============================================================================
// SUFFIX AUTOMATON FOR USERNAME SUBSTRING SEARCH
// ============================================================================

struct SAMState {
	int link;
	int len;
	std::unordered_map<char, int> next;
	std::vector<int> username_ids;  // Indices into all_usernames vector

	SAMState() : link(-1), len(0) {}
};

class SuffixAutomaton {
private:
	std::vector<SAMState> st;
	int last;
	int totalUsernames;

public:
	SuffixAutomaton();
	void addUsername(const std::string &s, int usernameID);
	void extend(char c, int usernameID);
	void finalize();
	std::vector<int> search(const std::string &pattern);
};

// Date: simple POD representing a calendar date.
// Intended for lightweight use (no timezone, no validation currently).
// Expand by adding validation, comparison operators or converting to std::chrono types if needed.
class Date {
private:
	int day;
	int month;
	int year;
public:
	// Constructor - default values set to 1/1/1970 so objects are always initialised.
	Date(int d = 0, int m = 0, int y = 0) : day(d), month(m), year(y) {}

	// Accessors
	int getDay() const { return day; }
	int getMonth() const { return month; }
	int getYear() const { return year; }

	// Mutators
	void setDay(int d) { day = d; }
	void setMonth(int m) { month = m; }
	void setYear(int y) { year = y; }

	// Convert date to string in dd/mm/yyyy form
	std::string toString() const { return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year); }
};

class UserHelper{
	public:
		// Function to encrypt the password of the user using hashmap
		static std::string encryptPassword(const std::string &username, const std::string &email, const std::string &password);

		// Email duplication tracking helpers
		bool isEmailRegistered(const std::string &email) const;
		void registerEmail(const std::string &email, const std::string &username) const;
		
		// Static hashmaps and vectors for graph features
		static std::unordered_map<std::string, std::string> email_to_username;
		static std::vector<std::string> all_usernames;  // Vector of all usernames for graph operations
		static Graph* friend_graph;  // Global friend relationship graph
		static SuggestionGraph* suggestion_graph;  // Runtime suggestion graph (rebuilt on each viewProfile)
		static SuffixAutomaton sam; // Suffix Automaton for username substring search
};
// Function to encrypt the password of the user using hashmap

#endif // HELPER_H
