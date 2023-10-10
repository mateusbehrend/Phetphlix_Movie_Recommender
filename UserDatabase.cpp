#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include "treemm.h"
using namespace std;

UserDatabase::UserDatabase()
{
    userTreeMultimap = new TreeMultimap<string, User>;
}

bool UserDatabase::load(const string& filename)
{
    std::ifstream usersFile(filename);
    std::string line;
    int moviesWatched;
    std::string name;
    std::string email;
    std::vector<std::string> movieID;
    while (std::getline(usersFile, line)) {
        name = line;
        std::getline(usersFile, line);
        email = line;
        std::getline(usersFile, line);
        moviesWatched = atoi(line.c_str());
        //clear the movieID vector to prepare it for the next user
        movieID.clear();
        for (int i = 0; i < moviesWatched; i++) {
            std::getline(usersFile, line);
            movieID.push_back(line);
        }

        std::getline(usersFile, line); //ignore the blank space between people
        //User* newUser = new User(name, email, movieID);

        userTreeMultimap->insert(email, User(name, email, movieID));
        //delete newUser; 
    }
    return true;
}

User* UserDatabase::get_user_from_email(const string& email) const
{
    User tempUser = (userTreeMultimap->find(email).get_value());
    //cerr << tempUser.get_full_name() << endl;
    //cerr << tempUser.get_watch_history()[0] << endl;
    //cerr << tempUser.get_watch_history()[1] << endl;
    return new User (tempUser);  
} 