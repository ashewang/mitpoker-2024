#include "equity_third_card.h"
#include <gtest/gtest.h>

#include "isomorphic_flop_encoder.h"

using namespace gtowizard;

TEST(EquityThirdCardTest, TestAvgEquityThirdCardTableIntegrity) {
  ASSERT_EQ(AVG_EQUITY_LOSS_THIRD_CARD.size(), 1755);
  ASSERT_EQ(ISOMORPHIC_FLOPS.size(), 1755);
  for (card_t i = 0; i < MAX_DECK_SIZE; ++i) {
    for (card_t j = i + 1; j < MAX_DECK_SIZE; ++j) {
      for (card_t k = j + 1; k < MAX_DECK_SIZE; ++k) {
        std::vector cards = {i, j, k};
        auto board = IsomorphicFlopEncoder::to_isomorphic_flop(cards);
        ASSERT_NE(std::find_if(ISOMORPHIC_FLOPS.cbegin(), ISOMORPHIC_FLOPS.cend(),
                               [&board](const auto& flop) {
                                 return std::strcmp(flop.data(), board.c_str()) == 0;
                               }),
                  ISOMORPHIC_FLOPS.cend());
        ASSERT_GE(AVG_EQUITY_LOSS_THIRD_CARD.at(board), -1.0);
        ASSERT_LT(AVG_EQUITY_LOSS_THIRD_CARD.at(board), 0);
      }
    }
  }
}

TEST(EquityThirdCardTest, TestHandEquitiesThirdCard) {
  HandEquitiesThirdCard hand_eqs;

  ASSERT_EQ(hand_eqs.raw_equities().size(),
            IsomorphicFlopEncoder::NUM_FLOPS * NUM_HANDS_POSTFLOP_2CARDS);
  for (auto equity : hand_eqs.raw_equities()) {
    ASSERT_GE(equity, -1.0);
    ASSERT_LE(equity, 0.0);
  }

  ASSERT_LT(hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("2c2d2h"), Hand("Ac2s")), 0);
  ASSERT_EQ(hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("2h2s3c"), Hand("Ac2s")), 0);
  ASSERT_EQ(hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("2h2s3c"), Hand("9c8c")),
            hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("2c2d3h"), Hand("9h8h")));
  ASSERT_NE(hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("2h2s3c"), Hand("9c8c")),
            hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("2c2d3h"), Hand("9s8s")));

  ASSERT_EQ(hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("5s7s8s"), Hand("9s8s")), 0);
  ASSERT_LT(hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("5s7s8s"), Hand("9c8c")), 0);
  ASSERT_EQ(hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("5s7s8s"), Hand("9c8c")),
            hand_eqs.get_hand_equity_loss_third_card(Card::to_vector("5c7c8c"), Hand("9s8s")));
}
