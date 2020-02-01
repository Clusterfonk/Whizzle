#ifndef PLAY_HAND_HEADER
#define PLAY_HAND_HEADER

#include "boost/algorithm/clamp.hpp"

#include "guess_card.h"
#include "play_card.h"

template <typename T>
class Hand {
public:
    Hand(float origin_y);
    ~Hand();

    T* update(sf::Window& window);
    void draw(sf::RenderTarget& target);

    bool add_card(T* new_card);
    void spawn();
    void discard();
    void discard(T* discard_card);
    size_t size();

    std::vector<T*>& get_cards();
private:
    std::vector<T*> cards;
    const unsigned int max_px_per_card = 80;
    const unsigned int min_px_per_card = 55;
    const float slope = (static_cast<float>(min_px_per_card) - static_cast<float>(max_px_per_card)) / static_cast<float>(550);
    const float constant = static_cast<float>(max_px_per_card) - slope * static_cast<float>(550);
    float calc_px_per_card(float distance_x);

    int delta_height = 8;
    float card_origin_y;
    sf::IntRect hand_position;
};

#endif
