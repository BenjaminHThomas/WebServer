#include <iostream>
#include <string>
#include <sstream>  // for std::stringstream

int main() {
    // Define the HTML body content
    std::string body = "<html>\n"
                       "<head>\n"
                       "<title>CGI Example</title>\n"
                       "</head>\n"
                       "<body>\n"
                       "<h1>Hello, World!</h1>\n"
                       "<p>This is a basic CGI program written in C++98.</p>\n"
                       "</body>\n"
                       "</html>\n";

    // Calculate content length (size of the HTML body)
    std::stringstream contentLengthStream;
    contentLengthStream << "Content-Length: " << body.length() << "\r\n";

    // Output the HTTP header
    std::cout << "HTTP/1.1 200 OK\r\n";
    std::cout << "Content-Type: text/html\r\n";
    std::cout << contentLengthStream.str();  // Add the Content-Length header
    std::cout << "\r\n";  // Empty line to separate headers from body

    // Output the HTML content
    std::cout << body;

    return 0;
}

