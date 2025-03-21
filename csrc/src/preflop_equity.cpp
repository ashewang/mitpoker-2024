#include "preflop_equity.h"

namespace gtowizard {

void compute_cfvs_preflop(const Range& opponent_range, float payoff,
                          std::vector<float>& postflop_cfvs,
                          const std::array<float, NUM_HANDS_PREFLOP * NUM_HANDS_PREFLOP>& payoffs) {
  if (opponent_range.num_cards != NumCards::Two) {
    throw std::invalid_argument("Can't call compute_cfvs_preflop with 3-cards range");
  }
  if (postflop_cfvs.size() < NUM_HANDS_POSTFLOP_2CARDS) {
    throw std::invalid_argument("Preflop CFVs have wrong size");
  }

  std::array<float, NUM_HANDS_PREFLOP> pf_range{};
  for (hand_t i = 0; i < NUM_HANDS_POSTFLOP_2CARDS; ++i) {
    pf_range[PREFLOP_HAND_IDX[i]] += opponent_range.range[i];
  }

  std::array<float, NUM_HANDS_PREFLOP> pf_cfvs{};

  for (hand_t i = 0; i < NUM_HANDS_PREFLOP; ++i) {
    for (hand_t j = 0; j < NUM_HANDS_PREFLOP; ++j) {
      pf_cfvs[i] += payoffs[i * NUM_HANDS_PREFLOP + j] * pf_range[j] * payoff;
    }
  }

  for (hand_t i = 0; i < NUM_HANDS_POSTFLOP_2CARDS; ++i) {
    postflop_cfvs[i] = pf_cfvs[PREFLOP_HAND_IDX[i]];
  }
}

}  // namespace gtowizard
