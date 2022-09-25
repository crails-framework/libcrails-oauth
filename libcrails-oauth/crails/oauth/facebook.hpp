#ifndef  CRAILS_OAUTH_FACEBOOK_API_HPP
# define CRAILS_OAUTH_FACEBOOK_API_HPP

# include "api.hpp"

namespace Crails
{
  namespace OAuth
  {
    class Facebook : public Api
    {
    public:
      Facebook(const OAuth::Settings& settings) : Api(settings) {}

      std::string get_user_email();
      std::string get_user_firstname();
      std::string get_user_lastname();
      std::string get_user_avatar();
    
      std::string access_token_url() const override;
      std::string get_oauth_url()    const override;
      std::string get_scope()        const override;
      std::string grant_type()       const override;
    private:
      void fetch_user_data() override;
    };
  }
}

#endif
