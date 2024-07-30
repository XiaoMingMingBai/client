#include <fmt/core.h>
#include <nlohmann/json.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Easy.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <list>
#include <thread>
#include <random>

class WebServer
{
private:
    std::string token;
    curlpp::Cleanup cleanup; // 初始化 cURLpp
    curlpp::Easy request;    // 创建一个 Request 对象
    std::string baseUrl = "http://localhost:8000/";

public:
    std::string url;
    std::string dat;
    std::string updateCpuUsage(float dat);
    WebServer(std::string userName, std::string passWord);
    ~WebServer();
};

WebServer::WebServer(std::string userName, std::string passWord)
{
    try
    {
        // 设置URL
        request.setOpt(new curlpp::options::Url(baseUrl + "user/login/"));

        // 数据json
        nlohmann::json datJson;
        datJson["username"] = "xiaomingmingbai";
        datJson["password"] = "love1999";

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
        std::string response;
        std::ostringstream responseStream;
        curlpp::options::WriteStream ws(&responseStream);
        request.setOpt(ws);
        request.perform();

        // 获取响应
        response = responseStream.str();
        std::cout << "Response: " << response << std::endl;

        // 解析json
        nlohmann::json responseJson = nlohmann::json::parse(response);
        token = responseJson["token"];
    }
    catch (curlpp::RuntimeError &e)
    {
        std::cerr << "RuntimeError: " << e.what() << std::endl;
    }
    catch (curlpp::LogicError &e)
    {
        std::cerr << "LogicError: " << e.what() << std::endl;
    }
}

std::string WebServer::updateCpuUsage(float dat)
{
    std::string response;

    try
    {
        // 设置URL
        request.setOpt(new curlpp::options::Url(baseUrl + "device/devices/1/"));

        // 数据json
        nlohmann::json datJson;
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(2) << dat;
        datJson["cpu_usage"] = stream.str();

        // 将 JSON 对象转换为字符串并打印
        std::string datjsonString = datJson.dump();
        std::cout << "PATCH json data: " << datjsonString << std::endl;

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
        std::ostringstream responseStream;
        curlpp::options::WriteStream ws(&responseStream);
        request.setOpt(ws);
        request.perform();

        // 获取响应
        response = responseStream.str();
        std::cout << "Response: " << response << std::endl;
    }
    catch (curlpp::RuntimeError &e)
    {
        std::cerr << "RuntimeError: " << e.what() << std::endl;
    }
    catch (curlpp::LogicError &e)
    {
        std::cerr << "LogicError: " << e.what() << std::endl;
    }
    return response;
}

WebServer::~WebServer()
{
}

int main()
{
    WebServer webServer("xiaomingmingbai", "love1999");

    while (true)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dis(20, 100);

        float randomNumber = static_cast<float>(dis(gen));
        webServer.updateCpuUsage(randomNumber);

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}