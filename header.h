#ifndef TEXAS_HOLDEM_UTIL_HEADER_H_
#define TEXAS_HOLDEM_UTIL_HEADER_H_

#include <algorithm>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>

namespace Base {

class Header {
    public:
        Header();
        ~Header();
        static Header parse(std::string str);
        static Header parse(std::istringstream iss);
        std::optional<std::string> get(std::string key) const;
        std::optional<std::string> erase(std::string key);
        template <typename T>
        void set(std::string key, T value) {
            if constexpr (std::is_same_v<bool, T>) {
                set(key, value ? "true" : "false");
            }
            else if constexpr (std::is_fundamental<T>::value) {
                set(key, std::to_string(value));
            }
            else {
                map_[key]   = value;
            }
        }
        void remove(std::string key);
        inline std::optional<std::string> operator[](std::string key) const {return get(key);}
        operator std::string() const {
            std::ostringstream oss;
            std::for_each(map_.begin(), map_.end(), [&oss](decltype(map_)::value_type pair) {
                    oss << pair.first << ": " << pair.second << "\x0d\x0a";
            });
            return oss.str();
        }
    private:
        std::unordered_map<std::string, std::string> map_;
};

}

#endif // TEXAS_HOLDEM_UTIL_HEADER_H_
