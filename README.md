# ALIEN: Fate of The Nostromo

This is a terminal-based clone of the ["ALIEN: Fate of The Nostromo" board game](https://boardgamegeek.com/boardgame/332321/alien-fate-nostromo), implemented in C.

## Installation

### Debug
Installs to `bin/aftn`
```sh
git clone https://github.com/CharlesAverill/aftn.git
cd aftn

cmake -S . -B build
cmake --build build -DDebugBuild=ON
```

### Full Install
Installs to `/usr/games/aftn`
```sh
git clone https://github.com/CharlesAverill/aftn.git
cd aftn

cmake -S . -B build
cmake --build build
sudo cmake --build build --target install -DDebugBuild=OFF
```

## Usage
```sh
Usage: aftn [OPTION...]

  -a, --use_ash              Include Ash for a more challenging game
  -c, --n_characters=integer Number of characters to create
  -d, --draw_map             Draw the game map if an ASCII map is provided
  -g, --game=FILE            Read game board from this path rather than the
                             default.
  -n, --n_players=integer    Number of players to create
  -p, --print_map            Print out a text representation of the game map
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```

Check [game_data/maps/format.txt](https://github.com/CharlesAverill/aftn/tree/main/game_data/maps/format.txt) to
create your own game boards
