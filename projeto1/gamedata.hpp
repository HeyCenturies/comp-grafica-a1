#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { W, S, Up, Down };
enum class State { Playing, Gol, VitJG1, VitJG2 };
enum class BallState { Playing, BallStop };

struct GameData {
  State m_state{State::Playing};
  BallState m_ballState{BallState::BallStop};
  std::bitset<4> m_input;
};

#endif