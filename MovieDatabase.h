#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include "treemm.h"
#include <string>
#include <vector>

class Movie;

class MovieDatabase
{
public:
    MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

private:
    TreeMultimap<std::string, Movie>* movieActorTreeMultimap;
    TreeMultimap<std::string, Movie>* movieIDTreeMultimap;
    TreeMultimap<std::string, Movie>* movieGenreTreeMultimap;
    TreeMultimap<std::string, Movie>* movieDirectorTreeMultimap;
    bool foundFile;
};

#endif // MOVIEDATABASE_INCLUDED