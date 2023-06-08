#include <crails/http.hpp>
#include <crails/utils/join.hpp>
#include "live.hpp"

using namespace Crails::OAuth;
using namespace std;

string Live::access_token_url() const
{
  return "https://login.live.com/oauth20_token.srf";
}

string Live::get_oauth_url() const
{
  return "https://login.live.com/oauth20_authorize.srf";
}

string Live::get_scope() const
{
  vector<string> scopes = {"wl.basic", "wl.emails"};

  return Crails::join(scopes.begin(), scopes.end(), ' ');
}

void Live::fetch_user_data()
{
  Crails::Ssl::Client client("api.live.net", 443);
  Crails::Client::Request request{
    Crails::HttpVerb::get,
    "/v5.0/me?access_token" + access_token.content,
    11
  };
  Crails::Client::Response response;
  DataTree result;

  client.connect();
  response = client.query(request);
  result.from_json(response.body());
  user_data = result;
}

std::string Live::get_user_email()
{
  DataTree& data = get_user_data();

  return data["emails"]["account"].defaults_to<string>("");
}

std::string Live::get_user_firstname()
{
  DataTree& data = get_user_data();

  return data["first_name"].defaults_to<string>("");
}

std::string Live::get_user_lastname()
{
  DataTree& data = get_user_data();

  return data["last_name"].defaults_to<string>("");
}

std::string Live::get_user_avatar()
{
  DataTree& data = get_user_data();

  return "https://apis.live.net/v5.0/" + data["id"].as<string>() + "/picture";
}
