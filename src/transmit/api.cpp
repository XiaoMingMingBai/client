#include <list>
#include <string>

#include "api.hpp"

#include <nlohmann/json.hpp> // json

#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>

WebServer::WebServer(std::string userName, std::string passWord)
{
    try
    {
        // 设置URL
        request.setOpt(new curlpp::options::Url(baseUrl + "user/login/"));

        // 数据json
        nlohmann::json datJson;
        datJson["username"] = userName;
        datJson["password"] = passWord;

        // 将 JSON 对象转换为字符串并打印
        std::string datjsonString = datJson.dump();
        std::cout << "POST json data: " << datjsonString << std::endl;

        // 设置post方法
        request.setOpt(new curlpp::options::PostFields(datjsonString));
        request.setOpt(new curlpp::options::PostFieldSize(datjsonString.length()));

        // 设置http头
        std::list<std::string> headers;
        headers.push_back("Content-Type: application/json");
        request.setOpt(new curlpp::options::HttpHeader(headers));

        // 执行请求
        std::string                  response;
        std::ostringstream           responseStream;
        curlpp::options::WriteStream ws(&responseStream);
        request.setOpt(ws);
        request.perform();

        // 获取响应
        response = responseStream.str();
        std::cout << "Response: " << response << std::endl;

        // 解析json
        nlohmann::json responseJson = nlohmann::json::parse(response);
        token                       = responseJson["token"];
    }
    catch (curlpp::RuntimeError& e)
    {
        std::cerr << "RuntimeError: " << e.what() << std::endl;
    }
    catch (curlpp::LogicError& e)
    {
        std::cerr << "LogicError: " << e.what() << std::endl;
    }
}

std::string WebServer::updataDeviceInfo(nlohmann::json datJson)
{
    std::string response;

    try
    {
        // 设置URL
        request.setOpt(new curlpp::options::Url(baseUrl + "device/devices/1/"));

        // 将 JSON 对象转换为字符串并打印
        std::string datjsonString = datJson.dump();

        // 设置PATCH方法
        request.setOpt(new curlpp::options::CustomRequest{"PATCH"});
        request.setOpt(new curlpp::options::PostFields(datjsonString));
        request.setOpt(new curlpp::options::PostFieldSize(datjsonString.length()));

        // 设置http头
        std::list<std::string> headers;
        headers.push_back("Content-Type: application/json");
        headers.push_back("Authorization: Token " + token);
        request.setOpt(new curlpp::options::HttpHeader(headers));

        // 执行请求
        std::ostringstream           responseStream;
        curlpp::options::WriteStream ws(&responseStream);
        request.setOpt(ws);
        request.perform();

        // 获取响应
        response = responseStream.str();
    }
    catch (curlpp::RuntimeError& e)
    {
        std::cerr << "RuntimeError: " << e.what() << std::endl;
    }
    catch (curlpp::LogicError& e)
    {
        std::cerr << "LogicError: " << e.what() << std::endl;
    }
    return response;
}

WebServer::~WebServer() {}
