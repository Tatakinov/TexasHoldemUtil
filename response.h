#ifndef TEXAS_HOLDEM_UTIL_RESPONSE_H_
#define TEXAS_HOLDEM_UTIL_RESPONSE_H_

#include <sstream>
#include <string>

#include "header.h"

namespace Base {

class Response {
    public:
        Response();
        Response(int code, const char *status, const char *protocol);
        ~Response();
        static Response parse(std::string str);
        static Response parse(std::istringstream iss);
        int getStatusCode() {return code_;}
        std::string getStatus() {return status_;}
        std::string getProtocol() {return protocol_;}
        void setStatusCode(int code) {code_ = code;}
        void setStatus(std::string status) {status_ = status;}
        void setProtocol(std::string protocol) {protocol_ = protocol;}
        template<typename T> void set(std::string key, T value) { header_.set(key, value); }
        std::optional<std::string> operator[](std::string key);
        operator std::string() const {
            std::ostringstream oss;
            oss << protocol_ << " " << code_ << " " << status_ << "\x0d\x0a";
            oss << static_cast<std::string>(header_);
            oss << "\x0d\x0a";
            return oss.str();
        }
    private:
        int code_;
        std::string status_;
        std::string protocol_;
        Header header_;
};

}

#endif // TEXAS_HOLDEM_UTIL_RESPONSE_H_
