#include <crails/http.hpp>
#include <crails/utils/join.hpp>
#include "facebook.hpp"

using namespace std;
using namespace Crails::OAuth;

string Facebook::access_token_url() const
{
  return "https://graph.facebook.com/oauth/access_token";
}

string Facebook::get_oauth_url() const
{
  return "https://www.facebook.com/dialog/oauth";
}

string Facebook::get_scope() const
{
  vector<string> scopes{"email","public_profile"};

  return Crails::join(scopes.begin(), scopes.end(), ' ');
}

string Facebook::grant_type() const
{
  return "client_credentials";
}

void Facebook::fetch_user_data()
{
}

string Facebook::get_user_email()
{
  return "";
}

string Facebook::get_user_firstname()
{
  return "";
}

string Facebook::get_user_lastname()
{
  return "";
}

string Facebook::get_user_avatar()
{
  return "";
}
