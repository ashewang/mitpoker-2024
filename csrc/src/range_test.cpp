#include "range.h"
#include <gtest/gtest.h>
#include "ranges_utils.h"

using namespace gtowizard;

class RangeTest : public ::testing::Test {
 protected:
  static constexpr float TOLERANCE = 1e-4;
  Game game_;
};

TEST_F(RangeTest, TestUniformRandomRange) {
  Range range;
  ASSERT_EQ(range.num_cards, NumCards::Two);
  ASSERT_NEAR(ranges::sum(range.range), n_choose_k(52, 2), TOLERANCE);
}

TEST_F(RangeTest, TestTo3CardsRange) {
  Range range;
  ASSERT_EQ(range.num_cards, NumCards::Two);

  range.to_3_cards_range(game_, {});
  ASSERT_EQ(range.num_cards, NumCards::Three);
  ASSERT_NEAR(ranges::sum(range.range), n_choose_k(52, 2), TOLERANCE);
}

TEST_F(RangeTest, TestUpdateOnNewBoardCards) {
  auto board_cards = Card::to_vector("AcAdAh");
  Range range;
  ASSERT_EQ(range.num_cards, NumCards::Two);

  range.update_on_board_cards(game_, board_cards);

  ASSERT_NEAR(ranges::sum(range.range), n_choose_k(49, 2), TOLERANCE);

  board_cards.push_back(Card("As").card());
  range.update_on_board_cards(game_, board_cards);

  ASSERT_NEAR(ranges::sum(range.range), n_choose_k(48, 2), TOLERANCE);

  board_cards.push_back(Card("2s").card());
  range.update_on_board_cards(game_, board_cards);

  ASSERT_NEAR(ranges::sum(range.range), n_choose_k(47, 2), TOLERANCE);

  ASSERT_EQ(range.num_cards, NumCards::Two);
}

TEST_F(RangeTest, TestTo3CardsRangeWithBoardCards) {
  const auto board_cards = Card::to_vector("AcAdAhAs");
  {
    Range range;

    range.to_3_cards_range(game_, board_cards);
    range.update_on_board_cards(game_, board_cards);

    ASSERT_EQ(range.num_cards, NumCards::Three);
    ASSERT_NEAR(ranges::sum(range.range), n_choose_k(48, 2), TOLERANCE);

    // Calling it again should still gave same result
    range.update_on_board_cards(game_, board_cards);
    ASSERT_NEAR(ranges::sum(range.range), n_choose_k(48, 2), TOLERANCE);
  }
  {  // Reverse order should give the same result (update->to_3_cards)
    Range range;

    range.update_on_board_cards(game_, board_cards);
    range.to_3_cards_range(game_, board_cards);

    ASSERT_EQ(range.num_cards, NumCards::Three);
    ASSERT_NEAR(ranges::sum(range.range), n_choose_k(48, 2), TOLERANCE);
  }
}
