#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    m_userDatabase =  &user_database;
    m_movieDatabase = &movie_database;
}

Recommender::~Recommender() {
    //destruct the dynamically allocated objects
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    vector<MovieAndRank> recommendedMovies;
    User* userObject = (m_userDatabase->get_user_from_email(user_email));
    //cerr << userObject->get_full_name() << endl;
    //cerr << userObject->get_email() << endl;
    //cerr << userObject->get_watch_history()[0] << endl;
    //Movie* movieObjecttest = (m_movieDatabase->get_movie_from_id(userObject->get_watch_history()[0]));
    //cerr << movieObjecttest->get_actors()[0] << endl;

    std::vector <std::string> userWatchHistory = userObject->get_watch_history();
    unordered_map<std::string, int> movieCSMap;
    for (int i = 0; i < (int)userWatchHistory.size();i++) {
        //set current MovieID
        std::string userObjectMovieID = userWatchHistory[i];
        //Get current movie from MovieID
        Movie* userCurMovie = m_movieDatabase->get_movie_from_id(userObjectMovieID);
        //Get directors,genres, and actors from Movie
        std::vector <std::string> curDirectorsOfMovie = userCurMovie->get_directors();
        std::vector <std::string> curGenresOfMovie = userCurMovie->get_genres();
        std::vector <std::string> curActorsOfMovie = userCurMovie->get_actors();
        
        // Map each movie to a value, to their compatability score
        //Assumption-- User's watch history has the same director there for multiple movies
        //it does still increment the compatibility score by 20 if the the movies with the same director
        //cerr << curDirectorOfMovie[i] << endl;
        for (int j = 0; j < curDirectorsOfMovie.size();j++) {
            vector <Movie*> movieIDsOfDirector = m_movieDatabase->get_movies_with_director(curDirectorsOfMovie[j]);
            for (int k = 0; k < (int)movieIDsOfDirector.size();k++) {
                string curMovieID = movieIDsOfDirector[k]->get_id();
                unordered_map<std::string, int>::iterator itDirectorMovieID = movieCSMap.find(curMovieID);
                if (curMovieID != userObjectMovieID) {
                    //item is found in the map
                    if (itDirectorMovieID != movieCSMap.end()) {
                        itDirectorMovieID->second += 20;
                    }
                    //item is not found in the map
                    else
                        movieCSMap.insert(make_pair(curMovieID, 20));
                }
            }
            //deallocate the dynamically constructed local objects
            for (int l = 0; l < (int)movieIDsOfDirector.size();l++) {
                delete movieIDsOfDirector[l];
            }
        }
        //same algorithum to increment based on actors
        for (int j = 0; j < curActorsOfMovie.size();j++) {
            vector <Movie*> movieIDsOfActors = m_movieDatabase->get_movies_with_actor(curActorsOfMovie[j]);
            for (int k = 0; k < (int)movieIDsOfActors.size();k++) {
                string curMovieID = movieIDsOfActors[k]->get_id();
                unordered_map<std::string, int>::iterator itActorMovieID = movieCSMap.find(curMovieID);
                //item is found in the map
                if (curMovieID != userObjectMovieID) {
                    if (itActorMovieID != movieCSMap.end()) {
                        itActorMovieID->second += 30;
                    }
                    //item is not found in the map
                    else
                        movieCSMap.insert(make_pair(curMovieID, 30));
                }
            }
            //deallocate the dynamically constructed local objects
            for (int l = 0; l < (int)movieIDsOfActors.size();l++) {
               delete movieIDsOfActors[l];
            }
        }

        for (int j = 0; j < curGenresOfMovie.size();j++) {
            vector <Movie*> movieIDsOfGenres = m_movieDatabase->get_movies_with_genre(curGenresOfMovie[j]);
            for (int k = 0; k < (int)movieIDsOfGenres.size();k++) {
                string curMovieID = movieIDsOfGenres[k]->get_id();
                unordered_map<std::string, int>::iterator itGenreMovieID = movieCSMap.find(curMovieID);
                if (curMovieID != userObjectMovieID) {
                    if (curMovieID != userObjectMovieID) {
                        //item is found in the map
                        if (itGenreMovieID != movieCSMap.end()) {
                            itGenreMovieID->second += 1;
                        }
                        //item is not found in the map
                        else
                            movieCSMap.insert(make_pair(curMovieID, 1));
                    }
                }
            }
            //deallocate the dynamically constructed local objects
            for (int l = 0; l < (int)movieIDsOfGenres.size();l++) {
                delete movieIDsOfGenres[l];
            }
        }
        delete userCurMovie;
    }
    //output all the data thats mapped --testing 
    //for (map<string, int>::iterator it = movieCSMap.begin();it != movieCSMap.end();it++) {
    //   cout << it->first << ": " << it->second << endl;
    //}
    //now sort the Objects into an array that contins the highest compatilbitily score
    //Sorting entire array would be O(NlogN)
    //Creating a quicksort 
    //Spec says to rank all the movies that a user has watched in the past, however we are only returning a vector of certain size
    // So I chose to use code to loop through and find max
    for (int i = 0; i < movie_count && (int)movieCSMap.size() > 0;i++ ) {
        int max = 0;
        string keyOfMax = "";
        for (int j = 0; j < movieCSMap.size();) {
            unordered_map<string, int>::iterator it = movieCSMap.begin();
            while (it != movieCSMap.end()) {
                if (it->second > max) {
                    max = it->second;
                    keyOfMax = it->first;
                }
                if (it->second == max) {
                    float newRating = m_movieDatabase->get_movie_from_id(it->first)->get_rating();
                    float oldMaxRating = m_movieDatabase->get_movie_from_id(keyOfMax)->get_rating();
                    if (newRating > oldMaxRating) {
                        max = it->second;
                        keyOfMax = it->first;
                    }
                    //if newRating is less, no new max is set
                    else if (newRating == oldMaxRating) {
                        string newName = m_movieDatabase->get_movie_from_id(it->first)->get_title();
                        string oldMaxName = m_movieDatabase->get_movie_from_id(keyOfMax)->get_title();
                        if (newName > oldMaxName) {
                            max = it->second;
                            keyOfMax = it->first;
                        }
                    }
                }
                it++;
                j++;
            }
            recommendedMovies.push_back(MovieAndRank(keyOfMax, max));
            movieCSMap.erase(keyOfMax);
        }
    }
    delete userObject;
    return recommendedMovies;
}
/*
void QuickSort(map<string, int> mapQuickSort, int First, int Last) {
    if (Last - First >= 1) {
        int PivotIndex;
        PivotIndex = Partition(mapQuickSort, First, Last);
        QuickSort(mapQuickSort, First, PivotIndex - 1);
        QuickSort(mapQuickSort, PivotIndex + 1, Last);
    }
}

int Partition(map<string, int> mapQuickSort, int low, int high) {
    map<string, int>::iterator itLow = mapQuickSort.begin();
    map<string, int>::iterator itLow = mapQuickSort.end();
    int pi = low;
    int pivot = itLow->second;
    itLow++;
    do {
        while (low <= high && it->second < pivot)
            it++;

    }
}
*/