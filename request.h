#ifndef TEXAS_HOLDEM_UTIL_REQUEST_H_
#define TEXAS_HOLDEM_UTIL_REQUEST_H_

#include <sstream>
#include <string>

#include "header.h"

namespace Base {

class Request {
    public:
        Request();
        Request(const char *command, const char *protocol);
        ~Request();
        static Request parse(std::string str);
        static Request parse(std::istringstream iss);
        std::string getCommand() {return command_;}
        std::string getProtocol() {return protocol_;}
        void setCommand(std::string command) {command_    = command;}
        void setProtocol(std::string protocol) {protocol_ = protocol;}
        template<typename T> void set(std::string key, T value) { header_.set(key, value); }
        std::optional<std::string> operator[](std::string key);
        operator std::string() const {
            std::ostringstream oss;
            oss << command_ << " " << protocol_ << "\x0d\x0a";
            oss << static_cast<std::string>(header_);
            oss << "\x0d\x0a";
            return oss.str();
        }

    private:
        std::string command_;
        std::string protocol_;
        Header header_;
};

}

#endif // TEXAS_HOLDEM_UTIL_REQUEST_H_
