#include <iostream>
#include <string>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

string getLocalIP() {
    struct ifaddrs *ifaddr, *ifa;
    string result = "Not found";
    
    if (getifaddrs(&ifaddr) == -1) {
        return "Error getting address";
    }
    
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;
        if (strcmp(ifa->ifa_name, "lo") == 0) continue;
        
        if (ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);
            result = string(ip);
            break;
        }
    }
    
    freeifaddrs(ifaddr);
    return result;
}

string httpGet(const string &host, const string &path) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return "";
    
    struct hostent *server = gethostbyname(host.c_str());
    if (server == nullptr) {
        close(sock);
        return "";
    }
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
    
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sock);
        return "";
    }
    
    string request = "GET " + path + " HTTP/1.0\r\nHost: " + host + "\r\n\r\n";
    send(sock, request.c_str(), request.length(), 0);
    
    string response;
    char buffer[1024];
    int bytes;
    
    while ((bytes = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes] = '\0';
        response += buffer;
    }
    
    close(sock);
    
    // Find body after headers
    size_t pos = response.find("\r\n\r\n");
    if (pos != string::npos) {
        return response.substr(pos + 4);
    }
    return response;
}

string getExternalIP() {
    string body = httpGet("icanhazip.com", "/");
    while (!body.empty() && (body.back() == '\n' || body.back() == '\r')) {
        body.pop_back();
    }
    return body.empty() ? "Not found" : body;
}

string extractJSONValue(const string &json, const string &key) {
    string searchKey = "\"" + key + "\": \"";
    size_t start = json.find(searchKey);
    if (start == string::npos) return "Unknown";
    
    start += searchKey.length();
    size_t end = json.find("\"", start);
    if (end == string::npos) return "Unknown";
    
    return json.substr(start, end - start);
}

int main(int argc, char *argv[]) {
    bool showGeo = false;
    
    if (argc > 1 && string(argv[1]) == "--geo") {
        showGeo = true;
    }
    
    cout << "Local IP:    " << getLocalIP() << endl;
    cout << "External IP: " << getExternalIP() << endl;
    
    if (showGeo) {
        string json = httpGet("ipinfo.io", "/json");
        
        if (!json.empty()) {
            string provider = extractJSONValue(json, "org");
            string country = extractJSONValue(json, "country");
            string city = extractJSONValue(json, "city");
            string region = extractJSONValue(json, "region");
            
            cout << "Provider:    " << provider << endl;
            cout << "Country:     " << country << endl;
            cout << "Region:      " << region << endl;
            cout << "City:        " << city << endl;
        } else {
            cout << "(Could not retrieve geo information)" << endl;
        }
    }
    
    return 0;
}