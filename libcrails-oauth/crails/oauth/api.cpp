#include <boost/lexical_cast.hpp>
#include <crails/url.hpp>
#include <crails/http.hpp>
#include <crails/logger.hpp>
#include "api.hpp"

using namespace Crails;
using namespace std;

OAuth::Api::Api(const Settings& settings) : settings(settings)
{
}

void OAuth::Api::require_access_token()
{
  if (access_token.content.size() == 0)
    fetch_access_token();
}

void OAuth::Api::require_user_data()
{
  if (!user_data)
  {
    require_access_token();
    fetch_user_data();
  }
}

void OAuth::Api::fetch_access_token()
{
  DataTree response_data;
  string body_string;
  stringstream body_stream;

  body_stream
    << "code="          << Crails::Url::encode(settings.code) << '&'
    << "client_id="     << Crails::Url::encode(settings.id) << '&'
    << "client_secret=" << Crails::Url::encode(settings.secret)  << '&'
    << "redirect_uri="  << Crails::Url::encode(settings.redirect_url) << '&'
    << "scope="         << Crails::Url::encode(get_scope()) << '&'
    << "grant_type=" << grant_type();
  body_string = body_stream.str();
  response_data.from_json(
    http_query(access_token_url(), body_stream.str(), false)
  );
  access_token.content    = response_data["access_token"].as<string>();
  access_token.expires_in = response_data["expires_in"].as<std::time_t>();
  access_token.type       = response_data["token_type"].as<string>();
}

DataTree& OAuth::Api::get_user_data()
{
  require_user_data();
  return *user_data;
}

string OAuth::Api::http_query(const std::string& url_string, const std::string& body, bool use_token)
{
  Url          url = Crails::Url::from_string(url_string);
  HttpRequest  request{HttpVerb::post, '/' + url.target, 11};
  HttpResponse response;
  std::unique_ptr<Client::ClientInterface> client;

  if (url.ssl)
    client.reset(new Crails::Ssl::Client(url.host, url.port));
  else
    client.reset(new Crails::Client(url.host, url.port));
  if (use_token)
    prepare(request);
  request.set(HttpHeader::host, url.host);
  request.set(HttpHeader::accept, "*/*");
  request.set(HttpHeader::connection, "close");
  request.set(HttpHeader::content_type, "application/x-www-form-urlencoded");
  request.content_length(body.length());
  request.body() = body;
  client->connect();
  logger << Logger::Debug << "OAuth(" << url_string << ") query:\n" << request << Logger::endl;
  response = client->query(request);
  logger << Logger::Debug << "OAuth(" << url_string << ") response:\n" << response << Logger::endl;
  return response.body();
}

void OAuth::Api::prepare(Crails::HttpRequest& request)
{
  stringstream authorization_header;

  authorization_header << access_token.type << ' ' << access_token.content;
  request.set(HttpHeader::authorization, authorization_header.str());
}
