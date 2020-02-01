#include "hand.h"

template <typename T>
Hand<T>::Hand(float origin_y) {
    card_origin_y = origin_y;
}

template <typename T>
Hand<T>::~Hand() {
}

template <typename T>
T* Hand<T>::update(sf::Window& window) {
    T* placed_card = nullptr;
    for(typename std::vector<T*>::reverse_iterator rit = cards.rbegin();
        rit != cards.rend(); ++rit) {
        placed_card = dynamic_cast<T*>((*rit)->update(window));
        if (placed_card) {
            return placed_card;
        }
    }
    return placed_card;
}

template <typename T>
void Hand<T>::draw(sf::RenderTarget &target) {
    for (auto card: cards) {
        card->draw(target);
    }
}

template <typename T>
bool Hand<T>::add_card(T* new_card) {
    if(!new_card) return false;
    cards.push_back(new_card);
    return true;
}

template <typename T>
void Hand<T>::spawn() {
    float px = calc_px_per_card(cards.size() * max_px_per_card);
    float card_origin_xe = (1280 + cards.size() * px) / 2 - px / 2;
    for(auto card: cards) {
        card->spawn({card_origin_xe, card_origin_y}, {1.0f, 1.0f});
        card_origin_xe -= px;
        delta_height *= -1;
        card_origin_y -= delta_height ;
    }
}

template <typename T>
void Hand<T>::discard(T *discard_card) {
    cards.erase(std::remove(cards.begin(), cards.end(), discard_card), cards.end());
}

template <typename T>
void Hand<T>::discard() {
    cards.clear();
}

template <typename T>
float Hand<T>::calc_px_per_card(float distance_x) {
    return boost::algorithm::clamp(slope * distance_x + constant, min_px_per_card, max_px_per_card);
}

template <typename T>
size_t Hand<T>::size() {
    return cards.size();
}

template <typename T>
std::vector<T*>& Hand<T>::get_cards() {
    return cards;
}

template class Hand<Play_card>;
template class Hand<Guess_card>;
