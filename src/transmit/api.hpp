#if !defined(__API_HPP__)
#define __API_HPP__

#include <string>

#include <curlpp/Easy.hpp>
#include <curlpp/cURLpp.hpp>
#include <nlohmann/json.hpp> // json

class WebServer
{
  private:
    std::string     token;
    curlpp::Cleanup cleanup;
    curlpp::Easy    request;
    std::string     baseUrl = "http://120.46.37.39:1214/";

  public:
    std::string url;
    std::string dat;

    std::string updataDeviceInfo(nlohmann::json datJson);
    WebServer(std::string userName, std::string passWord);
    ~WebServer();
};

#endif // __API_HPP__
