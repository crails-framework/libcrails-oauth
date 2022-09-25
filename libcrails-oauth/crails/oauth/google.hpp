#ifndef  CRAILS_OAUTH_GOOGLE_API_HPP
# define CRAILS_OAUTH_GOOGLE_API_HPP

# include "api.hpp"

namespace Crails
{
  namespace OAuth
  {
    class Google : public Api
    {
    public:
      Google(const OAuth::Settings& settings) : Api(settings) {}

      std::string get_user_email();
      std::string get_user_avatar();
      std::string get_user_firstname();
      std::string get_user_lastname();
      std::string get_user_name();
      std::string get_user_locale();

      std::string access_token_url() const override;
      std::string get_oauth_url()    const override;
      std::string get_scope()        const override;

    private:
      void fetch_user_data() override;
    };
  }
}

#endif
