#include "request.h"

namespace Base {

Request::Request() : command_(), protocol_(), header_() {}
Request::Request(const char *command, const char *protocol) : command_(command), protocol_(protocol), header_() {}
Request::~Request() {}

Request Request::parse(std::string str) {
    auto result = std::remove_if(str.begin(), str.end(),
            [](int ch) {return ch == '\x0d';}
    );
    str.erase(result, str.end());
    return parse(std::istringstream(str));
}

Request Request::parse(std::istringstream iss) {
    Request tmp;
    std::string line;
    getline(iss, line, '\x0a');
    decltype(line)::size_type pos    = line.rfind(' ');
    if (pos == decltype(line)::npos) {
        return tmp;
    }
    tmp.command_    = line.substr(0, pos);
    tmp.protocol_ = line.substr(pos + 1);
    tmp.header_     = Header::parse(std::move(iss));
    return tmp;
}

std::optional<std::string> Request::operator[](std::string key) {
    return header_[key];
}

}
