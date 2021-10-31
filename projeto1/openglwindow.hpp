#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>
#include <random>

#include "abcg.hpp"
#include "jogador.hpp"
#include "trave.hpp"
#include "bola.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  //GLuint m_traveProgram0{};
  GLuint m_traveProgram1{};
  GLuint m_traveProgram2{};
  GLuint m_jog1Program{};
  GLuint m_jog2Program{};
  GLuint m_bolaProgram{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  Jogador m_jog1;
  Jogador m_jog2;
  //Trave m_trave0;
  Trave m_trave1;
  Trave m_trave2;
  Bola m_bola;
  
  int directionX; 
  int directionY;
  int Jog1Score = 0;
  int Jog2Score = 0;

  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void bateu();
  void fezGol();

  void restart();
  void update();
};

#endif