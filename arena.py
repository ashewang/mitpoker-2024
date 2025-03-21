import math
import time
import json
import statistics
from dataclasses import dataclass
import config as config
from engine import Game, Player, BIG_BLIND


NUM_HANDS = 1000


@dataclass
class MatchResults:
    winrate: float  # in mbb/game
    stddev: float  # in mbb/game


def _run_match(player_1: Player, player_2: Player):
    """Based on `run()` in engine.py"""
    players = [player_1, player_2]
    game_clock = config.STARTING_GAME_CLOCK * NUM_HANDS / config.NUM_ROUNDS
    print(f"Setting game clock to {game_clock} seconds based on {NUM_HANDS} hands")
    for player in players:
        player.game_clock = game_clock

        start_time = time.perf_counter()
        player.build()
        end_time = time.perf_counter()

        print(f"Spent {end_time - start_time:.2f} seconds building bot {player.name}")

        full_output = "".join([str(output) for output in player_1.bytes_queue.queue])
        if "Timed out waiting" in full_output:
            raise TimeoutError("Timed out waiting for bot to build/connect")

        player.run()

    game = Game()
    winnings_per_hand = []
    prev_bankroll = player_1.bankroll
    for _ in range(NUM_HANDS):
        game.run_round(players)
        winnings_per_hand.append(player_1.bankroll - prev_bankroll)
        prev_bankroll = player_1.bankroll
        # Switch players position
        players = players[::-1]

    for player in players:
        player.stop()
        print(
            f"Player {player.name} spent {game_clock - player.game_clock:.2f}s playing (used {100.0 * (game_clock - player.game_clock) / game_clock:.2f}% of his time)"
        )
        if player.game_clock <= 1e-10:
            raise TimeoutError(f"Bot {player.name} ran out of time when playing")

    assert len(winnings_per_hand) == NUM_HANDS

    bb_per_hand = [x / float(BIG_BLIND) for x in winnings_per_hand]
    stddev_match = math.sqrt(NUM_HANDS * statistics.variance(bb_per_hand))
    results = MatchResults(
        winrate=statistics.mean(bb_per_hand),
        stddev=stddev_match / NUM_HANDS,
    )

    print(f"Results: {results.winrate:.2f} ± {results.stddev:.2f} bb/hand")

    return results


def run_match_vs_check_call_bot():
    main_bot = Player(name="main_vs_check_or_call", path="./csrc")
    check_call_bot = Player(name="check_or_call", path="./simple_bots/check_or_call_bot")
    results = _run_match(main_bot, check_call_bot)

    return {
        "name": "Results vs. Check/Call Bot",
        "unit": "bb/hand",
        "value": round(results.winrate, 4),
        "range": round(results.stddev, 4),
    }


def run_match_vs_bid_everything_bot():
    main_bot = Player(name="main_vs_bid_everything", path="./csrc")
    bid_everything_bot = Player(
        name="bid_everything_bot", path="./simple_bots/bid_everything_bot"
    )
    results = _run_match(main_bot, bid_everything_bot)

    return {
        "name": "Results vs. Bid Everything Bot",
        "unit": "bb/hand",
        "value": round(results.winrate, 4),
        "range": round(results.stddev, 4),
    }


def run_match_vs_uniform_random_bot():
    main_bot = Player(name="main_vs_uniform_random", path="./csrc")
    uniform_random_bot = Player(name="uniform_random", path="./simple_bots/uniform_random_bot")
    results = _run_match(main_bot, uniform_random_bot)

    return {
        "name": "Results vs. Uniform Random Bot",
        "unit": "bb/hand",
        "value": round(results.winrate, 4),
        "range": round(results.stddev, 4),
    }

def run_match_vs_preflop_all_in_bot():
    main_bot = Player(name="main_vs_preflop_all_in", path="./csrc")
    preflop_all_in_bot = Player(
        name="preflop_all_in_bot", path="./simple_bots/preflop_all_in_bot"
    )
    results = _run_match(main_bot, preflop_all_in_bot)

    return {
        "name": "Results vs. Preflop All-in Bot",
        "unit": "bb/hand",
        "value": round(results.winrate, 4),
        "range": round(results.stddev, 4),
    }

def run_match_vs_main_bot():
    """
    Match against ourselves, mostly as an end-to-end test and make sure we don't crash
    when reaching some parts of the game tree that won't be reached with "dumb" bots
    """
    main_bot = Player(name="main_vs_main", path="./csrc")
    main_bot_mirror = Player(name="main_vs_main_mirror", path="./csrc")
    results = _run_match(main_bot, main_bot_mirror)

    return {
        "name": "Results vs. Ourselves",
        "unit": "bb/hand",
        "value": round(results.winrate, 4),
        "range": round(results.stddev, 4),
    }

def main():
    results = [
        run_match_vs_check_call_bot(),
        run_match_vs_bid_everything_bot(),
        run_match_vs_uniform_random_bot(),
        run_match_vs_preflop_all_in_bot(),
        run_match_vs_main_bot(),
    ]

    with open("arena_results.json", "w") as f:
        json.dump(results, f, indent=4)


if __name__ == "__main__":
    main()
