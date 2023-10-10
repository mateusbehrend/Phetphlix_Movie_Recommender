#include "User.h"

#include <string>
#include <vector>
using namespace std;

User::User(const string& full_name, const string& email,
           const vector<string>& watch_history)
{
    // Initialize variables that match values that User is initialized with
    m_userFullName = full_name;
    m_userEmail = email;
    m_userWatchHistory = watch_history;
}

string User::get_full_name() const
{
    return m_userFullName;  // Replace this line with correct code.
}

string User::get_email() const
{
    return m_userEmail;  // Replace this line with correct code.
}

vector<string> User::get_watch_history() const
{
    return m_userWatchHistory;  // Replace this line with correct code.
}
