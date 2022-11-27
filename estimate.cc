#include "estimate.h"

#include <algorithm>
#include <deque>
#include <sstream>
#include <random>

constexpr int kLoopSize = 1000;

static std::random_device rd;
static std::mt19937 engine(rd());

bool equal(hand_t& a, hand_t& b);

bool lessThan(hand_t& a, hand_t& b);

card_t str2card(std::string str) {
    card_t card = {};
    card.suit = str[0];
    std::istringstream iss(str.substr(1));
    iss >> card.number;
    return card;
}

double estimate(std::vector<card_t> community, int size, std::vector<player_t> players) {
    std::deque<card_t> deck;
    for (int i = 1; i <= 13; i++) {
        deck.push_back({'S', i});
        deck.push_back({'H', i});
        deck.push_back({'C', i});
        deck.push_back({'D', i});
    }
    // 既知のカードは取り除く
    auto it = std::remove_if(deck.begin(), deck.end(), [&](card_t a) {
        for (auto& e : community) {
            if (e.suit == a.suit && e.number == a.number) {
                return true;
            }
        }
        for (auto& e1 : players) {
            for (auto& e2 : e1.cards) {
                if (e2.suit == a.suit && e2.number == a.number) {
                    return true;
                }
            }
        }
        return false;
    });
    deck.erase(it, deck.end());

    int num_winning = 0;
    std::vector<winner_t> winners;
    winners.reserve(players.size());

    for (int i = 0; i < kLoopSize; i++) {
        auto d    = deck;
        auto f    = community;
        auto p    = players;

        std::shuffle(d.begin(), d.end(), engine);

        // プレイヤー全員にカードを行き渡らせる
        for (int i = 0; i < p.size(); i++) {
            if (p[i].cards[0].number == 0) {
                p[i].cards[0] = d.front();
                d.pop_front();
            }
            if (p[i].cards[1].number == 0) {
                p[i].cards[1] = d.front();
                d.pop_front();
            }
        }
        for (int i = p.size(); i < size; i++) {
            player_t player;
            player.cards[0] = d.front();
            d.pop_front();
            player.cards[1] = d.front();
            d.pop_front();
            p.push_back(player);
        }
        // フロップ・ターン・リバーの5枚になるように
        for (int i = f.size(); i < 5; i++) {
            f.push_back(d.front());
            d.pop_front();
        }

        winners.clear();

        for (int i = 0; i < p.size(); i++) {
            std::vector<card_t> cards = {
                    p[i].cards[0], p[i].cards[1],
                    f[0], f[1], f[2], f[3], f[4]
            };

            auto hand = evaluate(cards);

            if (winners.size() == 0) {
                winners.push_back({i, hand});
            }
            else {
                if (equal(winners[0].hand, hand)) {
                    winners.push_back({i, hand});
                }
                else if (lessThan(winners[0].hand, hand)) {
                    winners.clear();
                    break;
                }
            }
        }
        for (auto& e : winners) {
            if (e.index == 0) {
                num_winning++;
                break;
            }
        }
    }
    return 100.0 * num_winning / kLoopSize;
}

bool equal(hand_t& a, hand_t& b) {
    if (a.main != b.main) {
        return false;
    }
    for (int i = 0; i < a.sub.size(); i++) {
        if (a.sub[i] != b.sub[i]) {
            return false;
        }
    }
    return true;
}

bool lessThan(hand_t& a, hand_t& b) {
    if (a.main < b.main) {
        return true;
    }
    else if (a.main > b.main) {
        return false;
    }
    for (int i = 0; i < a.sub.size(); i++) {
        if (a.sub[i] < b.sub[i]) {
            return true;
        }
        else if (a.sub [i] > b.sub[i]) {
            return false;
        }
    }
    return false;
}

int isStraight(int array[]) {
    int num = -1;
    int seq = 0;
    for (int n = 1; n < 16; n++) {
        if (array[n]) {
            seq++;
        }
        else {
            if (seq >= 5) {
                num = n;
            }
            seq = 0;
        }
    }
    if (num == -1) {
        if (array[14] &&
                array[5] &&
                array[4] &&
                array[3] &&
                array[2]
           ) {
            return 5;
        }
    }
    return num;
}

hand_t evaluate(std::vector<card_t>& cards) {
    for (int i = 0; i < cards.size(); i++) {
        if (cards[i].number == 1) {
            cards[i].number = 14;
        }
    }
    std::sort(cards.begin(), cards.end(), [](card_t a, card_t b) {
            return a.number > b.number;
    });
    hand_t result = {None, {}};
    result.sub.reserve(5);

    int suit_map[4][16]     = {};
    for (auto& e : cards) {
        if (e.suit == 'S') {
            suit_map[0][e.number]++;
        }
        else if (e.suit == 'H') {
            suit_map[1][e.number]++;
        }
        else if (e.suit == 'C') {
            suit_map[2][e.number]++;
        }
        else if (e.suit == 'D') {
            suit_map[3][e.number]++;
        }
    }
    for (int i = 0; i < 4; i++) {
        int sub = isStraight(suit_map[i]);
        if (sub > 0) {
            result.main = StraightFlush;
            result.sub.push_back(sub);
            return result;
        }
    }

    int map[16] = {};
    for (auto& e : cards) {
        map[e.number]++;
    }
    for (int i = 16 - 1; i > 0; i--) {
        if (map[i] == 4) {
            result.main = FourOfAKind;
            result.sub.clear();
            result.sub.push_back(i);
            for (int j = 16 - 1; j > 0; j--) {
                if (map[j] && i != j) {
                    result.sub.push_back(j);
                    return result;
                }
            }
            break;
        }
        else if (map[i] == 3) {
            result.sub.push_back(i);
            if (result.main == ThreeOfAKind) {
                result.main = FullHouse;
                return result;
            }
            result.main = ThreeOfAKind;
            break;
        }
    }
    for (int i = 16 - 1; i > 0; i--) {
        if (map[i] == 2) {
            result.sub.push_back(i);
            switch (result.main) {
                case ThreeOfAKind:
                    result.main = FullHouse;
                    return result;
                    break;
                case OnePair:
                    result.main = TwoPair;
                    for (int j = 16 - 1; j >= i; j--) {
                        if (map[j] == 1) {
                            result.sub.push_back(j);
                            return result;
                        }
                    }
                    for (int j = i - 1; j > 0; j--) {
                        if (map[j]) {
                            result.sub.push_back(j);
                            return result;
                        }
                    }
                    __builtin_unreachable();
                    break;
                case None:
                    result.main = OnePair;
                    break;
                default:
                    __builtin_unreachable();
                    break;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        std::vector<int> sub;
        sub.reserve(5);
        for (int n = 16 - 1; n > 0; n--) {
            if (suit_map[i][n]) {
                sub.push_back(n);
                if (result.sub.size() == 5) {
                    break;
                }
            }
        }
        if (sub.size() == 5) {
            result.main = Flush;
            result.sub  = sub;
            return result;
        }
    }
    int n = isStraight(map);
    if (n > 0) {
        result.main = Straight;
        result.sub.clear();
        result.sub.push_back(n);
        return result;
    }
    if (result.main == None) {
        result.main = HighCard;
    }
    int limit   = -1;
    switch (result.main) {
        case ThreeOfAKind:
            limit   = 3;
            break;
        case OnePair:
            limit   = 4;
            break;
        case HighCard:
            limit   = 5;
            break;
        default:
            __builtin_unreachable();
            break;
    }
    for (int i = 16 - 1; i > 0; i--) {
        if (map[i] == 1) {
            if (result.sub.size() < limit) {
                result.sub.push_back(i);
                if (result.sub.size() == limit) {
                    break;
                }
            }
        }
    }
    return result;
}

