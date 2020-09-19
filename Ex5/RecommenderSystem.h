/**
 * @file RecommenderSystem.h
 * @author Ron Shuvy
 * @brief This file defines a Movie Recommendation System
 */

#ifndef RECOMMENDERSYSTEM_H
#define RECOMMENDERSYSTEM_H
// ------------------------------ includes ------------------------------
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
// -------------------------- const definitions -------------------------
#define NA 0
#define SUCCESS 0
#define FAILURE -1
// ------------------------------ class declaration -----------------------------

/**
 * @class RecommenderSystem
 * @brief This class represents a Movie Recommendation System
 */
class RecommenderSystem
{
private:
    std::unordered_map<std::string, std::vector<double>> _movies; // movie scores
    std::unordered_map<std::string, std::vector<double>> _users; // user grades
    std::vector<std::string> _titlesInOrder; // movie titles listed by input order

    /**
    * @brief Loads movie data from a given file
    */
    int _loadMovies(const std::string& moviesAttributedFilePath);

    /**
    * @brief Loads users data from a given file
    */
    int _loadUsers(const std::string& userRanksFilePath);

public:

    /**
    * @brief Loads movies and users data from an input files
    */
    int loadData(const std::string& moviesAttributedFilePath, const std::string& userRanksFilePath);

    /**
    * @brief Return a recommended movie title for a given user by using Content-related Algorithm
    */
    std::string recommendByContent(const std::string& userName) const;

    /**
    * @brief Return a score prediction of a given movie and a given user
    */
    double predictMovieScoreForUser(const std::string& movieName, const std::string& userName, int k) const;

    /**
    * @brief Return a recommended movie title for a given user by using Collaborative Filtering
    */
    std::string recommendByCF(const std::string& userName, int k) const;
};

#endif //RECOMMENDERSYSTEM_H



