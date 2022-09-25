#ifndef  OAUTH_CONTROLLER_HPP
# define OAUTH_CONTROLLER_HPP

# include "api.hpp"
# include <crails/controller.hpp>
# include <crails/odb/controller.hpp> // would be cool to be database agnostic...
# include <crails/signin/session.hpp>

# define oauth_actions(path, controller) \
  match_action("GET", std::string(path), controller, create) \
 .match_action("GET", std::string(path) + "/callback", controller, callback)

namespace Crails::OAuth
{
  template<class USER, class API, class SUPER = Crails::Odb::Controller<>>
  class Controller : public SUPER
  {
  public:
    Controller(Crails::Context& context) :
      SUPER(context),
      current_session(SUPER::database, SUPER::session)
    {
    }

    virtual Settings              settings() const = 0;
    virtual std::shared_ptr<USER> find_user(const std::string& email) = 0;
    virtual std::shared_ptr<USER> create_user(API& api) = 0;
    virtual void                  update_user(API& api, USER& user) = 0;
    virtual bool                  is_acceptable_new_user(API& api) const { return true; }
    virtual bool                  must_protect_from_forgery() const override { return false; }

    void create()
    {
      API api(settings());
      std::string redirect_url = api.get_oauth_url();
      Data return_to = SUPER::params["return_to"];

      redirect_url += "?response_type=code";
      redirect_url += "&scope="        + api.get_scope();
      redirect_url += "&client_id="    + api.get_oauth_id();
      redirect_url += "&redirect_uri=" + api.get_redirect_url();
      SUPER::redirect_to(HttpStatus::temporary_redirect, redirect_url);
      SUPER::session["return_to"] = return_to.defaults_to<std::string>("");
    }

    void callback()
    {
      using namespace std;
      if (SUPER::params["code"].exists())
      {
        shared_ptr<USER> user;
        string           email;
        API              api(settings().with_code(SUPER::params["code"]));
        Data             return_to = SUPER::session["return_to"];

        email = api.get_user_email();
        user  = find_user(email);
        if (!user && is_acceptable_new_user(api))
          user = create_user(api);
        else if (user != nullptr)
          update_user(api, *user);
        else
        {
          SUPER::respond_with(HttpStatus::forbidden);
          return ;
        }
        current_session.set_current_user(user);
        if (return_to.exists())
        {
          SUPER::redirect_to(return_to.as<std::string>());
          return_to.destroy();
        }
        else
          SUPER::redirect_to("/");
      }
      else
        SUPER::respond_with(HttpStatus::bad_request);
    }

  private:
    Session<USER> current_session;
  };
}

#endif
