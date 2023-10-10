#include "MovieDatabase.h"
#include "Movie.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
using namespace std;

MovieDatabase::MovieDatabase()
{
    movieIDTreeMultimap = new TreeMultimap<string, Movie>;
    movieActorTreeMultimap = new TreeMultimap<string, Movie>;
    movieDirectorTreeMultimap = new TreeMultimap<string, Movie>;
    movieGenreTreeMultimap = new TreeMultimap<string, Movie>;
    foundFile = false;
}

bool MovieDatabase::load(const string& filename)
{
    std::ifstream movieFile(filename);
    std::string line;
    std::string movieID;
    std::string movieName;
    std::string movieReleaseYear;
    std::vector<std::string> directors;
    std::vector<std::string> actors;
    std::vector<std::string> genre;
    float rating;
    while (std::getline(movieFile, line)) {
        int previous = 0;
        movieID = line;
        std::getline(movieFile, line);
        movieName = line;
        std::getline(movieFile, line);
        movieReleaseYear = line;
        std::getline(movieFile, line);
        previous = 0;
        for (int i = 0; i < (int)line.size();i++) {
            if (line[i] == ',') {
                directors.push_back(line.substr(previous, i - previous));
                //movieDirectorTreeMultimap->insert(line.substr(previous, i - previous), movieID);
                //cerr << line.substr(previous, i - previous) << endl;
                previous = i + 1;
            }
        }

        directors.push_back(line.substr(previous, line.size() - previous)); //be used to insert first and/or last
        //movieDirectorTreeMultimap->insert(line.substr(previous, line.size()- previous), movieID);
        //cerr << line.substr(previous, line.size()) << endl;
        //assemble actors vector
        std::getline(movieFile, line);
        previous = 0;
        for (int i = 0; i < (int)line.size();i++) {
            if (line[i] == ',') {
                actors.push_back(line.substr(previous, i - previous));
                //movieActorTreeMultimap->insert(line.substr(previous, i - previous), movieID);
                //cout << line.substr(previous, i-previous) << endl;
                previous = i + 1;
            }
        }
        actors.push_back(line.substr(previous, line.size() - previous)); //need to add the last actor
        //movieActorTreeMultimap->insert(line.substr(previous, line.size() - previous), movieID);
        //cout << line.substr(previous, line.size() - previous) << endl;

        std::getline(movieFile, line);
        previous = 0;
        for (int i = 0; i < (int)line.size();i++) {
            if (line[i] == ',') {
                genre.push_back(line.substr(previous, i - previous));
                //movieGenreTreeMultimap->insert(line.substr(previous, i - previous), movieID);
                //cerr << line.substr(previous, i - previous) << endl;
                previous = i + 1;
            }
        }
        genre.push_back(line.substr(previous, line.size() - previous));
        //movieGenreTreeMultimap->insert(line.substr(previous, line.size() - previous), movieID);
        //cerr << line.substr(previous, line.size() - previous) << endl;
        std::getline(movieFile, line);
        rating = stof(line);
        //Movie* newMovie = new Movie(movieID,movieName, movieReleaseYear, directors, actors, genre, rating);
        movieIDTreeMultimap->insert(movieID, Movie(movieID, movieName, movieReleaseYear, directors, actors, genre, rating));
        for (int i = 0; i < (int)actors.size();i++) {
            movieActorTreeMultimap->insert(actors[i], Movie(movieID, movieName, movieReleaseYear, directors, actors, genre, rating));
        }
        for (int i = 0; i < (int)genre.size();i++) {
            movieGenreTreeMultimap->insert(genre[i], Movie(movieID, movieName, movieReleaseYear, directors, actors, genre, rating));
        }
        for (int i = 0; i < (int)directors.size();i++) {
            movieDirectorTreeMultimap->insert(directors[i], Movie(movieID, movieName, movieReleaseYear, directors, actors, genre, rating));
        }
        //clear all the vectors for the current movie, and prepare vectors for next movie
        directors.clear();
        actors.clear();
        genre.clear();
        //delete newMovie;
        //read in the blank space
        std::getline(movieFile, line);
        foundFile = true;
    }
    if (foundFile)
        return true;
    else
        return false;
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    Movie tempMovie = (movieIDTreeMultimap->find(id).get_value());
    return new Movie(tempMovie);
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector<Movie*>directorMoviesVector;
    //one single time we need to return the iterator which has contains a vector of the objects we are trying to get
    //we do this by finding that single element O(logD)
    TreeMultimap<std::string, Movie>::Iterator moviesOfDirectorIt = movieDirectorTreeMultimap->find(director);
    //iterate through rest of values that moviesOfDirector has
    while (moviesOfDirectorIt.is_valid()) {
        directorMoviesVector.push_back(new Movie(moviesOfDirectorIt.get_value()));
        moviesOfDirectorIt.advance();
    }
    //test if directorMoviesVector contains the data needed
    //for (int i = 0; i < directorMoviesVector.size();i++)
    //    cerr << directorMoviesVector[i]->get_actors()[i] << endl;
    return directorMoviesVector;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    static vector<Movie*>actorsMoviesVector;
    actorsMoviesVector.clear();
    //one single time we need to return the iterator which has contains a vector of the objects we are trying to get
    //we do this by finding that single element O(logA)
    TreeMultimap<std::string, Movie>::Iterator moviesOfActorIt = movieActorTreeMultimap->find(actor);
    //iterate through rest of values that moviesOfActor has
    while (moviesOfActorIt.is_valid()) {
        actorsMoviesVector.push_back(new Movie(moviesOfActorIt.get_value()));
        moviesOfActorIt.advance();
    }
    //checking to make sure it works 
    //--assumption checked on eva longoria, but doesn't pick up the data after she was married and changed her name to eva longoria parker
    //so assumed as if they are two other people
    //for (int i = 0; i < (int)actorsMoviesVector.size();i++)
    //    cerr << ((actorsMoviesVector[i])->get_title()) << endl;

    return actorsMoviesVector;
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    vector<Movie*>genreMoviesVector;
    //one single time we need to return the iterator which has contains a vector of the objects we are trying to get
    //we do this by finding that single element O(logA)
    TreeMultimap<std::string, Movie>::Iterator moviesOfGenreIt = movieGenreTreeMultimap->find(genre);
    //iterate through rest of values that moviesOfGenreVectorIt has
    while (moviesOfGenreIt.is_valid()) {
        genreMoviesVector.push_back(new Movie(moviesOfGenreIt.get_value()));
        moviesOfGenreIt.advance();
    }
    return genreMoviesVector;
}
