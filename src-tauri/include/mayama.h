#pragma once
#include <cstdint>
#include <memory>

enum class GameStatus : std::uint8_t;

class Game {
public:
  Game();
  Game(Game &&) = default;
  Game(const Game &) = default;
  Game &operator=(Game &&) = default;
  Game &operator=(const Game &) = default;
  ~Game();

  GameStatus get_status() const noexcept;

private:
};

std::unique_ptr<Game> new_game();
