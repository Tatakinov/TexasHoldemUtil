#include "saori.h"

#include <iostream>
#include <optional>
#include <sstream>

#include "request.h"
#include "response.h"
#include "estimate.h"

bool __load(std::string path) {
    return true;
}

bool __unload() {
    return true;
}

std::string __request(std::string request) {
    Base::Request req = Base::Request::parse(request);
    if (req.getCommand() == "GET Version") {
        Base::Response res{204, "No Content", "SAORI/1.0"};
        return res;
    }
    if ( ! req["Argument0"]) {
        Base::Response res{400, "Bad Request", "SAORI/1.0"};
        return res;
    }
    std::string command = req["Argument0"].value();
    if (command == "hand") {
        std::vector<card_t> list;
        for (int i = 1; ; i++) {
            std::ostringstream oss;
            oss << "Argument" << i;
            if (req[oss.str()]) {
                list.push_back(str2card(req[oss.str()].value()));
            }
            else {
                break;
            }
        }
        for (int i = list.size(); i < 5; i++) {
            list.push_back({'N', 0});
        }
        auto ret    = evaluate(list);
        Base::Response res{200, "OK", "SAORI/1.0"};
        res.set("Result", static_cast<int>(ret.main));
        for (int i = 0; i < ret.sub.size(); i++) {
            std::ostringstream oss;
            oss << "Value" << i;
            res.set(oss.str(), ret.sub[i]);
        }
        return res;
    }
    else if (command == "estimate") {
        if ( ! req["Argument1"]) {
            Base::Response res{400, "Bad Request", "SAORI/1.0"};
            return res;
        }
        int num_of_community;
        std::vector<card_t> community;
        int num_of_player;
        std::vector<player_t> list;
        {
            std::istringstream iss(req["Argument1"].value());
            iss >> num_of_community;
        }
        int index = 2;
        for (; index - 2 < num_of_community; index++) {
            std::ostringstream oss;
            oss << "Argument" << index;
            if (req[oss.str()]) {
                community.push_back(str2card(req[oss.str()].value()));
            }
            else {
                std::cout << "invalid community number" << std::endl;
                goto invalid;
            }
        }
        {
            std::ostringstream oss;
            oss << "Argument" << index;
            if (req[oss.str()]) {
                std::istringstream iss(req[oss.str()].value());
                iss >> num_of_player;
                if (num_of_player > 10) {
                    goto invalid;
                    std::cout << "invalid num_of_player" << std::endl;
                }
            }
            else {
                goto invalid;
            }
            index++;
        }
        for (int i = index; i - index < num_of_player * 2; i+=2) {
            std::string tmp;
            {
                std::ostringstream oss;
                oss << "Argument" << i;
                if (req[oss.str()]) {
                    tmp = req[oss.str()].value();
                }
                else {
                    break;
                }
            }
            {
                std::ostringstream oss;
                oss << "Argument" << (i + 1);
                if (req[oss.str()]) {
                    player_t player;
                    player.cards[0] = str2card(tmp);
                    player.cards[1] = str2card(req[oss.str()].value());
                    list.push_back(player);
                }
                else {
                    player_t player;
                    player.cards[0] = str2card(tmp);
                    // 2枚目はNull相当の値を入れる。
                    player.cards[1] = str2card("N0");
                    list.push_back(player);
                    break;
                }
            }
        }
        auto rate = estimate(community, num_of_player, list);
        Base::Response res{200, "OK", "SAORI/1.0"};
        res.set("Result", rate);
        return res;
    }
invalid:
    Base::Response res{200, "OK", "SAORI/1.0"};
    res.set("Result", -1);
    return res;
}
