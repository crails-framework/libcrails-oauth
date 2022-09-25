#ifndef  OAUTH_API_HPP
# define OAUTH_API_HPP

# include <crails/client.hpp>
# include <crails/datatree.hpp>
# include <crails/http.hpp>
# include <optional>

namespace Crails
{
  namespace OAuth
  {
    struct Settings
    {
      std::string id;
      std::string secret;
      std::string redirect_url;
      std::string code;

      Settings& with_id(const std::string& value) { id = value; return *this; }
      Settings& with_secret(const std::string& value) { secret = value; return *this; }
      Settings& with_redirect_url(const std::string& value) { redirect_url = value; return *this; }
      Settings& with_code(const std::string& value) { code = value; return *this; }
    };

    class Api
    {
      struct AccessToken
      {
        std::string type;
        std::string content;
        std::time_t expires_in;
      };

    public:
      Api(const Settings&);

      void fetch_access_token();
      DataTree& get_user_data();

      virtual std::string access_token_url() const = 0;
      virtual std::string get_oauth_url() const = 0;
      std::string         get_oauth_id() const { return settings.id; }
      std::string         get_oauth_secret() const { return settings.secret; }
      std::string         get_redirect_url() const { return settings.redirect_url; }
      virtual std::string get_scope() const = 0;
      virtual std::string grant_type() const { return "authorization_code"; }

      virtual void fetch_user_data() = 0;

    protected:
      void require_access_token();
      void require_user_data();
      void prepare(Crails::HttpRequest&);
      std::string http_query(const std::string& url, bool use_token = true)
      { return http_query(url, "", use_token); }
      std::string http_query(const std::string& url, const std::string& body, bool use_token = true);

      Settings                settings;
      AccessToken             access_token;
      std::optional<DataTree> user_data;
    };
  }
}

#endif
