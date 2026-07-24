/*
 * simpleI — Simple IP Info
 * A lightweight terminal tool that shows local and external IP addresses.
 * Licensed under GNU GPL v3.0 or later.
 */

#include <iostream>
#include <string>
#include <ifaddrs.h>      // for getifaddrs() — local network interfaces
#include <netinet/in.h>   // for sockaddr_in, AF_INET
#include <arpa/inet.h>    // for inet_ntop() — convert IP to string
#include <cstring>         // for strcmp()
#include <sys/socket.h>   // for socket(), connect(), send(), recv()
#include <netdb.h>        // for gethostbyname()
#include <unistd.h>       // for close()

using namespace std;

/*
 * getLocalIP — returns the local IPv4 address of the first non-loopback interface.
 * Uses getifaddrs() to list all network interfaces and picks the first IPv4 address
 * that is not 127.0.0.1.
 */
string getLocalIP() {
    struct ifaddrs *ifaddr, *ifa;
    string result = "Not found";
    
    // Get list of all network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        return "Error getting address";
    }
    
    // Walk through the linked list of interfaces
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) continue;
        
        // Skip loopback interface (127.0.0.1)
        if (strcmp(ifa->ifa_name, "lo") == 0) continue;
        
        // We only want IPv4 addresses
        if (ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *addr = (struct sockaddr_in *)ifa->ifa_addr;
            char ip[INET_ADDRSTRLEN];
            // Convert binary IP to human-readable string
            inet_ntop(AF_INET, &(addr->sin_addr), ip, INET_ADDRSTRLEN);
            result = string(ip);
            break;  // Take the first matching interface
        }
    }
    
    freeifaddrs(ifaddr);  // Free the interface list
    return result;
}

/*
 * httpGet — performs a simple HTTP GET request and returns the response body.
 * host: the server to connect to (e.g., "icanhazip.com")
 * path: the path to request (e.g., "/")
 * 
 * This is a minimal HTTP/1.0 client using raw sockets. No external libraries.
 */
string httpGet(const string &host, const string &path) {
    // Create a TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return "";
    
    // Resolve hostname to IP address
    struct hostent *server = gethostbyname(host.c_str());
    if (server == nullptr) {
        close(sock);
        return "";
    }
    
    // Set up the server address structure
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);  // HTTP port
    memcpy(&addr.sin_addr.s_addr, server->h_addr, server->h_length);
    
    // Connect to the server
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        close(sock);
        return "";
    }
    
    // Build and send a minimal HTTP/1.0 GET request
    string request = "GET " + path + " HTTP/1.0\r\nHost: " + host + "\r\n\r\n";
    send(sock, request.c_str(), request.length(), 0);
    
    // Receive the response
    string response;
    char buffer[1024];
    int bytes;
    
    while ((bytes = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes] = '\0';
        response += buffer;
    }
    
    close(sock);
    
    // HTTP response: headers followed by blank line, then body
    // We want only the body
    size_t pos = response.find("\r\n\r\n");
    if (pos != string::npos) {
        return response.substr(pos + 4);
    }
    return response;
}

/*
 * getExternalIP — returns the external (public) IP address.
 * Uses icanhazip.com which returns a plain-text IP address.
 */
string getExternalIP() {
    string body = httpGet("icanhazip.com", "/");
    // Remove trailing newline characters
    while (!body.empty() && (body.back() == '\n' || body.back() == '\r')) {
        body.pop_back();
    }
    return body.empty() ? "Not found" : body;
}

/*
 * extractJSONValue — extracts a string value from a simple JSON object.
 * json: the raw JSON string
 * key: the key to look for (e.g., "country")
 * 
 * This is a minimal parser. It looks for "key": "value" and returns value.
 * Works correctly for the simple JSON returned by ipinfo.io.
 */
string extractJSONValue(const string &json, const string &key) {
    string searchKey = "\"" + key + "\": \"";
    size_t start = json.find(searchKey);
    if (start == string::npos) return "Unknown";
    
    start += searchKey.length();
    size_t end = json.find("\"", start);
    if (end == string::npos) return "Unknown";
    
    return json.substr(start, end - start);
}

/*
 * main — entry point.
 * Without arguments: shows local and external IP.
 * With --geo flag: also shows provider, country, region, and city.
 */
int main(int argc, char *argv[]) {
    bool showGeo = false;
    
    // Check for --geo flag
    if (argc > 1 && string(argv[1]) == "--geo") {
        showGeo = true;
    }
    
    // Always show IP addresses
    cout << "Local IP:    " << getLocalIP() << endl;
    cout << "External IP: " << getExternalIP() << endl;
    
    // Show geo information only if requested
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
