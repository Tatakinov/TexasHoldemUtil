#ifndef TEXAS_HOLDEM_UTIL_ESTIMATE_H_
#define TEXAS_HOLDEM_UTIL_ESTIMATE_H_

#include <string>
#include <vector>

enum Hand {
    None,
    HighCard,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    Straight,
    Flush,
    FullHouse,
    FourOfAKind,
    StraightFlush
};

typedef struct {
    char suit;
    int number;
} card_t;

typedef struct {
    card_t cards[2];
} player_t;

typedef struct {
    Hand main;
    std::vector<int> sub;
} hand_t;

typedef struct {
    int index;
    hand_t hand;
} winner_t;

card_t str2card(std::string str);

hand_t evaluate(std::vector<card_t>& cards);

double estimate(std::vector<card_t> community, int size, std::vector<player_t> cards);

#endif // TEXAS_HOLDEM_UTIL_ESTIMATE_H_
