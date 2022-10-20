#include "Buff.h"

#include <random>

Buff::Buff() {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> ti(1, 3);
    tier = ti(rng);
    std::string types = "RC";
    std::uniform_int_distribution<std::mt19937::result_type> ty(0, types.length() - 1);
    type = types[ty(rng)];
    charSymbol = type;
}

Buff::Buff(char t)
: type(t) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<std::mt19937::result_type> ti(1, 3);

    tier = ti(rng);

    charSymbol = t;
}

Buff::Buff(uint8_t t)
: tier ( t ) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::string types = "RC";
    std::uniform_int_distribution<std::mt19937::result_type> ty(0, types.length() - 1);
    type = types[ty(rng)];

    charSymbol = type;
}

Buff::Buff(uint8_t tier, char type)
: type(type), tier(tier) {
    charSymbol = type;
}

bool Buff::tick() {
    return !(--duration);
}
