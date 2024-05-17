#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <glog/logging.h>
#include <string.h>

#define LOCAL_PORT 8000

using namespace std;
using namespace google;

int g_clientSocked;
string g_recvBuf;
string g_sendBuf;

int InitGoogleLogService(const char *argv0)
{
    google::InitGoogleLogging(argv0);
    FLAGS_logtostderr = 1;
    FLAGS_log_dir = "./log/";

    LOG(INFO) << "Init Google Log Service Success!"
              << " log_dir: " << FLAGS_log_dir;

    return 0;
}

int InitSockedService(const char *argv1, const char *argv2)
{
    int ret;

    g_clientSocked = socket(AF_INET, SOCK_STREAM, 0);
    if (g_clientSocked < 0)
    {
        LOG(ERROR) << "Create Socked Failed!";
        return g_clientSocked;
    }
    LOG(INFO) << "Create Socked Success!";

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(LOCAL_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(argv2);
    ret = connect(g_clientSocked, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret < 0)
    {
        LOG(ERROR) << "Connect Socked Failed!";
        return ret;
    }
    LOG(INFO) << "Connect Socked Success!";
    return 0;
}

int main(int argc, char *argv[])
{
    int ret;

#ifdef NDEBUG
    LOG(INFO) << "Release Version!";
#else
    LOG(INFO) << "Debug Version!";
#endif

    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <server_ip>" << endl;
        return -1;
    }

    InitGoogleLogService(argv[0]);
    InitSockedService(argv[1], argv[2]);

    while (1)
    {   g_sendBuf.clear();
        cout << "Please input message: ";
        cin >> g_sendBuf;
        send(g_clientSocked, g_sendBuf.c_str(), g_sendBuf.size(), 0);
        LOG(INFO) << "Send Message: " << g_sendBuf;

        g_recvBuf.clear();
        g_recvBuf.resize(1024);
        ret = recv(g_clientSocked, (char *)g_recvBuf.c_str(), g_recvBuf.size(), 0);
        if (ret < 0)
        {
            LOG(ERROR) << "Recv Message Failed!";
            return ret;
        }
        g_recvBuf.resize(ret);
        LOG(INFO) << "Recv Message: " << g_recvBuf;
    }

    return 0;
}