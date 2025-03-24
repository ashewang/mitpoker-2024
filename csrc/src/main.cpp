#include "../src/main_bot.h"
#include "../src/runner.h"
#include "../src/definitions.h"
using namespace gtowizard;

std::array<int, 2> NO_BETS = {0, 0};
std::array<std::optional<int>, 2> NO_BIDS = {std::nullopt, std::nullopt};
std::array<std::string, MAX_RANKS> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
std::array<std::string, MAX_SUITS> suits = {"c", "d", "h", "s"};
/*
  Main program for running our pokerbot
*/
int main(int argc, char* argv[]) {
  auto [host, port] = parseArgs(argc, argv);
  // runBot<MainBot>(host, port);
  MainBot bot;
  GameInfo game_info(/*bankroll=*/0, /*game_clock=*/10.0, /*num_hands_in_match=*/1000);

  std::vector<std::string> all_hands;
  for (int r1 = 0; r1 < MAX_RANKS; r1++) {
    for (int s1 = 0; s1 < MAX_SUITS; s1++) {
      for (int r2 = 0; r2 < MAX_RANKS; r2++) {
        for (int s2 = 0; s2 < MAX_SUITS; s2++) {
          if (r1 == r2 && s1 == s2) continue;
          all_hands.push_back(ranks[r1] + suits[s1] + ranks[r2] + suits[s2]);
        }
      }
    }
  }
  fmt::print("Solving for {} hands\n", all_hands.size());
  
  for (auto hand : all_hands) {
    std::array<std::string, 2> hands = {hand, ""};
    StatePtr round_state = std::make_shared<RoundState>(0, false, NO_BIDS, BLINDS, STARTING_STACKS,
                                                        hands, std::vector<card_t>{}, nullptr);

    auto action =
        bot.get_action(game_info, std::static_pointer_cast<const RoundState>(round_state), 0);
    fmt::print("{} - {} \n", hands[0], action.to_string());
  }
  return 0;
}
 