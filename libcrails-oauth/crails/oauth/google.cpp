#include <crails/http.hpp>
#include <crails/logger.hpp>
#include "google.hpp"

using namespace std;
using namespace Crails::OAuth;

string Google::access_token_url() const
{
  return "https://www.googleapis.com/oauth2/v4/token";
}

string Google::get_oauth_url() const
{
  return "https://accounts.google.com/o/oauth2/auth";
}

string Google::get_scope() const
{
  return "openid email profile";
}

void Google::fetch_user_data()
{
  DataTree result;

  result.from_json(
    http_query("https://openidconnect.googleapis.com/v1/userinfo")
  );
  user_data = result;
  Crails::logger << "OAuth user data: " << result.to_json() << Crails::Logger::endl;
}

string Google::get_user_email()
{
  DataTree& data = get_user_data();

  if (!data["email"].exists())
    throw std::runtime_error("[Google] No email found for current user");
  return data["email"];
}

string Google::get_user_name()
{
  DataTree& data = get_user_data();

  return data["name"].defaults_to<string>("");
}

string Google::get_user_firstname()
{
  DataTree& data = get_user_data();

  return data["given_name"].defaults_to<string>("");
}

string Google::get_user_lastname()
{
  DataTree& data = get_user_data();

  return data["family_name"].defaults_to<string>("");
}

string Google::get_user_locale()
{
  DataTree& data = get_user_data();

  return data["locale"].defaults_to<string>("");
}

string Google::get_user_avatar()
{
  DataTree& data = get_user_data();

  return data["picture"].defaults_to<string>("");
}
