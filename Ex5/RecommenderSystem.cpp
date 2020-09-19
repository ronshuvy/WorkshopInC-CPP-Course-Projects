/**
 * @file RecommenderSystem.cpp
 * @author Ron Shuvy
 * @brief This file implements a Movie Recommendation System
 */

// ------------------------------ includes ------------------------------
#include "RecommenderSystem.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
// -------------------------- const definitions -------------------------
#define WRONG_PATH(filePath) "Unable to open file " #filePath
#define READING_ERROR "Unable to read file completely"
#define USER_NOT_FOUND "USER NOT FOUND"
#define NOT_ACTIVE "NA"
#define INT_MIN -2
// ------------------------------ utilities -----------------------------
using std::vector, std::multimap, std::string;

/**
* @brief Multiplies a vector by a scalar
*/
vector<double> operator*(vector<double> v, double c)
{
    for_each(v.begin(), v.end(), [c](double &val){ val *= c; });
    return v;
}

/**
* @brief Sums two vectors
*/
vector<double> operator+(const vector<double>& v1, const vector<double>& v2)
{
    assert(v1.size() == v2.size());
    vector<double> sum(v1.size());
    for (size_t i = 0; i < v1.size() ; i++)
    {
        sum[i] = v1[i] + v2[i];
    }
    return sum;
}

/**
* @brief Adds rhs vector to lhs vector
*/
vector<double>& operator+=(vector<double>& v1, const vector<double>& v2)
{
    if (v1.empty())
    {
        v1.resize(v2.size());
    }
    else
    {
        assert(v1.size() == v2.size());
    }
    for (size_t i = 0; i < v1.size() ; i++)
    {
        v1[i] += v2[i];
    }
    return v1;
}

/**
* @brief Scalar multiplication between two vectors
*/
double innerProduct(const vector<double> &v1, const vector<double> &v2)
{
    double sum = 0;
    for (size_t i = 0; i < v1.size(); ++i)
    {
        sum += (v1[i] * v2[i]);
    }
    return sum;
}

/**
* @brief Normalizes a list of vectors
*/
vector<double> normalizeVec(const vector<double> &vec)
{
    vector<double> normVec(vec);
    double sum = 0, count = 0;
    for (const auto& movieGrade : normVec)
    {
        if (movieGrade != NA)
        {
            sum += movieGrade;
            count++;
        }
    }
    double norm = sum / count;
    for (auto& movieGrade : normVec)
    {
        if (movieGrade != NA)
        {
            movieGrade -= norm;
        }
    }
    return normVec;
}

/**
* @brief Calculated the norm a given vector
*/
double getNorm(const vector<double> &v)
{
    return sqrt(innerProduct(v, v));
}

/**
* @brief Calculates the angle between two vectors
*/
double computeAngle(const vector<double> &v1, const vector<double> &v2)
{
    return innerProduct(v1, v2) / (getNorm(v1) * getNorm(v2));
}

// ------------------------------ class definitions -----------------------------

/**
* @brief Return a recommended movie title for a given user by using Content-related Algorithm
*/
string RecommenderSystem::recommendByContent(const string& userName) const
{
    if (!_users.count(userName))
    {
        return USER_NOT_FOUND;
    }
    // Stage 1
    vector<double> normVec = normalizeVec(_users.at(userName));

    //Stage 2
    vector<double> prefVec;
    for (size_t i = 0; i < normVec.size() ; ++i)
    {
        if (normVec[i] != NA)
        {
            string movieName = _titlesInOrder[i];
            prefVec += _movies.at(movieName) * normVec[i];
        }
    }

    //Stage 3
    string recommendMovie;
    double maxAngle = INT_MIN;
    for (size_t i = 0; i < normVec.size() ; ++i)
    {
        if (_users.at(userName)[i] == NA)
        {
            string movieName = _titlesInOrder[i];
            double angle = computeAngle(prefVec, _movies.at(movieName));
            if (angle > maxAngle)
            {
                recommendMovie = movieName; maxAngle = angle;
            }
        }
    }
    return recommendMovie;
}

/**
* @brief Return a score prediction of a given movie and a given user
*/
double RecommenderSystem::predictMovieScoreForUser(const string& movieName, const string& userName, int k) const
{
    if (!_users.count(userName) || !_movies.count(movieName))
    {
        return FAILURE;
    }

    multimap<double, string, std::greater<>> kSimilarMovies;
    for (size_t i = 0; i < _users.at(userName).size() ; ++i)
    {
        if (_users.at(userName)[i] != NA)
        {
            string currMovie = _titlesInOrder[i];
            double angle = computeAngle(_movies.at(movieName), _movies.at(currMovie));
            kSimilarMovies.insert(std::pair<double, string>(angle, currMovie));
        }
    }
    kSimilarMovies.erase(prev(kSimilarMovies.end(), kSimilarMovies.size()-k), kSimilarMovies.end());

    double numerator = 0, denominator = 0;
    for (const auto& movie : kSimilarMovies)
    {
        string movName = movie.second;
        auto itr = find(_titlesInOrder.begin(), _titlesInOrder.end(), movName);
        double movRate = _users.at(userName)[distance(_titlesInOrder.begin(), itr)];
        numerator += (movie.first * movRate);
        denominator += movie.first;
    }
    return numerator / denominator;
}

/**
* @brief Return a recommended movie title for a given user by using Collaborative Filtering
*/
string RecommenderSystem::recommendByCF(const string& userName, int k) const
{
    if (!_users.count(userName))
    {
        return USER_NOT_FOUND;
    }

    double bestScore = INT_MIN;
    string bestMovie;
    for (size_t i = 0; i < _users.at(userName).size(); i++)
    {
        string currMovie = _titlesInOrder[i];
        if (_users.at(userName)[i] == NA)
        {
            double currScore = predictMovieScoreForUser(currMovie, userName, k);
            if (currScore > bestScore)
            {
                bestScore = currScore;
                bestMovie = currMovie;
            }
        }
    }
    return bestMovie;
}

/**
* @brief Loads users data from a given file
*/
int RecommenderSystem::_loadUsers(const string &userRanksFilePath)
{
    std::ifstream file(userRanksFilePath);
    string line;
    if (!file.is_open())
    {
        std::cerr << WRONG_PATH(userRanksFilePath);
        return FAILURE;
    }

    // Load movie titles
    getline(file, line);
    std::istringstream lineReader(line, std::ios::binary);
    string title;
    while (lineReader >> title)
    {
        _titlesInOrder.push_back(title);
    }

    // Load user graders
    while (getline(file, line))
    {
        std::istringstream usersReader(line, std::ios::binary);
        string userName;
        usersReader >> userName;
        string userScore;
        double score;
        int i = 0;
        while (usersReader >> userScore)
        {
            score = (userScore == NOT_ACTIVE) ? NA : stod(userScore);
            string movie = _titlesInOrder[i];
            _users[userName].push_back(score);
            i++;
        }
    }
    if (!file.eof())
    {
        std::cerr << READING_ERROR;
        return FAILURE;
    }
    file.close();
    return SUCCESS;
}

/**
* @brief Loads movie data from a given file
*/
int RecommenderSystem::_loadMovies(const string &moviesAttributedFilePath)
{
    std::ifstream file(moviesAttributedFilePath);
    string line;
    if (!file.is_open())
    {
        std::cerr << WRONG_PATH(moviesAttributedFilePath);
        return FAILURE;
    }
    while (getline(file, line))
    {
        std::istringstream lineReader(line, std::ios::binary);
        string movieName;
        lineReader >> movieName;
        int score;
        while (lineReader >> score)
        {
            _movies[movieName].push_back(score);
        }
    }
    if (!file.eof())
    {
        std::cerr << READING_ERROR;
        return FAILURE;
    }
    file.close();
    return SUCCESS;
}

/**
* @brief Loads movies and users data from an input files
*/
int RecommenderSystem::loadData(const string& moviesAttributedFilePath, const string& userRanksFilePath)
{
    return ((_loadMovies(moviesAttributedFilePath) == SUCCESS) && (_loadUsers(userRanksFilePath) == SUCCESS))
    ? SUCCESS : FAILURE;
}



