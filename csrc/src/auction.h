#pragma once
#include "equity_third_card.h"
#include "game.h"
#include "hand.h"
#include "range.h"

namespace gtowizard {

class Auctioneer {
 public:
  Auctioneer();

  int v_abs_bid_min_max[2]{};
  float v_pot_percentage_min_max[2]{};
  bool v_is_excessive_bidder;
  int bid_count = 0;

  int get_bid(const std::vector<card_t>& board_cards, const Hand& hand, int pot);

  void receive_bid(Range& hero_range, Range& villain_range, int hero_bid, int villain_bid,
                   const Game& game, const std::vector<card_t>& board_cards, int pot);

  void update_exploits(int hero_bid, int villain_bid, int bid_plus_pot);

 private:
  HandEquitiesThirdCard hand_equities_third_card_;
};

}  // namespace gtowizard
