#include "../src/main_bot.h"
#include "../src/runner.h"
#include "../src/definitions.h"
using namespace gtowizard;

std::array<int, 2> NO_BETS = {0, 0};
std::array<std::optional<int>, 2> NO_BIDS = {std::nullopt, std::nullopt};
/*
  Main program for running our pokerbot
*/
int main(int argc, char* argv[]) {
  auto [host, port] = parseArgs(argc, argv);
  // runBot<MainBot>(host, port);
  MainBot bot;
  GameInfo game_info(/*bankroll=*/0, /*game_clock=*/10.0, /*num_hands_in_match=*/1000);

  for (auto hand : std::vector<std::string>{"AcAd", "9s8s", "2c3c", "2d7c"}) {
    std::array<std::string, 2> hands = {hand, ""};
    StatePtr round_state = std::make_shared<RoundState>(0, false, NO_BIDS, BLINDS, STARTING_STACKS,
                                                        hands, std::vector<card_t>{}, nullptr);

    auto action =
        bot.get_action(game_info, std::static_pointer_cast<const RoundState>(round_state), 0);
    fmt::print("{} - {} \n", hands[0], action.to_string());
  }
  return 0;
}
 