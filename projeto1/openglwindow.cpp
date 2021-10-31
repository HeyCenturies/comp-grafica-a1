#include <imgui.h>

#include "openglwindow.hpp"
#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_w && m_jog1.m_translation.y < 0.75f){      
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    }
    if (event.key.keysym.sym == SDLK_s && m_jog1.m_translation.y > -0.75f){
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    }
    if (event.key.keysym.sym == SDLK_UP && m_jog2.m_translation.y < 0.75f){
      m_gameData.m_input.set(static_cast<size_t>(Input::W));
    }
    if (event.key.keysym.sym == SDLK_DOWN && m_jog2.m_translation.y > -0.75f){
      m_gameData.m_input.set(static_cast<size_t>(Input::S));
    }
  }

  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_UP)
      m_gameData.m_input.reset(static_cast<size_t>(Input::W));
    if (event.key.keysym.sym == SDLK_DOWN)
      m_gameData.m_input.reset(static_cast<size_t>(Input::S));
  }
}

void OpenGLWindow::initializeGL() {
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath()+ "Inconsolata-Medium.ttf"};

  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 65.0f);

  m_jog1Program   = createProgramFromFile(getAssetsPath()+"obj.vert", getAssetsPath()+"obj.frag");       
  m_jog2Program   = createProgramFromFile(getAssetsPath()+"obj.vert", getAssetsPath()+"obj.frag");          
  m_bolaProgram   = createProgramFromFile(getAssetsPath()+"obj.vert", getAssetsPath()+"obj.frag");
  //m_traveProgram0 = createProgramFromFile(getAssetsPath()+"obj.vert", getAssetsPath()+"obj.frag");
  m_traveProgram1 = createProgramFromFile(getAssetsPath()+"obj.vert", getAssetsPath()+"obj.frag");
  m_traveProgram2 = createProgramFromFile(getAssetsPath()+"obj.vert", getAssetsPath()+"obj.frag");
  glClearColor(0, 0.7, 0, 1);

#if !defined(__EMSCRIPTEN__)
  glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;

  m_jog1.initializeGL(m_jog1Program);
  m_jog2.initializeGL(m_jog2Program);

  //m_trave0.initializeGL(m_traveProgram0);
  m_trave1.initializeGL(m_traveProgram1);
  m_trave2.initializeGL(m_traveProgram2);

  //m_trave0.m_translation = glm::vec2(0.01f, 0);
  m_trave1.m_translation = glm::vec2(-0.96f, 0);
  m_trave2.m_translation = glm::vec2(0.96f, 0);
  m_jog2.m_translation   = glm::vec2(0.7f, 0);

  //direcao inicial
  directionX = 1;
  directionY = 0;

  m_bola.initializeGL(m_bolaProgram);
}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};
  
  if (m_gameData.m_state == State::VitJG1 && m_restartWaitTimer.elapsed() > 4) {
    restart();
    return;
  }
  else if (m_gameData.m_state == State::VitJG2 && m_restartWaitTimer.elapsed() > 4) {
    restart();
    return;
  }
  else if (m_gameData.m_state == State::Gol && m_restartWaitTimer.elapsed() > 3) {
    restart();
    return;
  }

  if (m_gameData.m_state == State::Playing) {
    bateu();
    fezGol();
  }

  //controles
  if (m_gameData.m_input[static_cast<size_t>(Input::Up)] && m_jog1.m_translation.y < 0.8f){
    m_jog1.setTranslation(glm::vec2(m_jog1.m_translation.x, m_jog1.m_translation.y + (0.8 * deltaTime)));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::Down)] && m_jog1.m_translation.y > -0.8f){
    m_jog1.setTranslation(glm::vec2(m_jog1.m_translation.x, m_jog1.m_translation.y - (0.8 * deltaTime)));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::W)] && m_jog2.m_translation.y < 0.8f){
    m_jog2.setTranslation(glm::vec2(m_jog2.m_translation.x, m_jog2.m_translation.y + (0.8 * deltaTime)));
  }
  if (m_gameData.m_input[static_cast<size_t>(Input::S)] && m_jog2.m_translation.y > -0.8f){
    m_jog2.setTranslation(glm::vec2(m_jog2.m_translation.x, m_jog2.m_translation.y - (0.8 * deltaTime)));
  }
  m_bola.update(deltaTime, directionX, directionY);
}

void OpenGLWindow::paintGL() {
  update();
  glClear(GL_COLOR_BUFFER_BIT);

  m_jog1.paintGL(m_gameData);
  m_jog2.paintGL(m_gameData);
  m_bola.paintGL(m_gameData);
  //m_trave0.paintGL(m_gameData);
  m_trave1.paintGL(m_gameData);
  m_trave2.paintGL(m_gameData);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
   //score window
  {
    auto size{ImVec2(350, 450)};
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::Gol) {
      
      ImGui::Text("%d x %d", Jog1Score, Jog2Score);
    } 
    if (m_gameData.m_state == State::VitJG1) {
       ImGui::Text("Jogador 1");
       ImGui::Text("ganhou!");
    }
    if (m_gameData.m_state == State::VitJG2) {
       ImGui::Text("Jogador 2");
       ImGui::Text("ganhou!");
    }
    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  glDeleteProgram(m_jog1Program);
  glDeleteProgram(m_jog2Program);
  glDeleteProgram(m_bolaProgram);
  //glDeleteProgram(m_traveProgram0);
  glDeleteProgram(m_traveProgram1);
  glDeleteProgram(m_traveProgram2);

  m_jog1.terminateGL();
  m_jog2.terminateGL();
  m_bola.terminateGL();
  //m_trave0.terminateGL();
  m_trave1.terminateGL();
  m_trave2.terminateGL();
}

void OpenGLWindow::bateu() {
  float distXJog1 = abs(m_jog1.m_translation.x - m_bola.m_translation.x) <= 0.05f;
  float distXJog2 = abs(m_jog2.m_translation.x - m_bola.m_translation.x) <= 0.05f; 
  float distYJog1 = abs(m_jog1.m_translation.y - m_bola.m_translation.y) <= 0.1f;
  float distYJog2 = abs(m_jog2.m_translation.y - m_bola.m_translation.y) <= 0.1f;
  
  if(distXJog1 && distYJog1){      //prox jogador 1
      if(m_jog1.m_translation.y == m_bola.m_translation.y)    {directionY = 0;}  //tocou no meio
      else if(m_jog1.m_translation.y > m_bola.m_translation.y){directionY = -1;} //tocou em baixo
      else if(m_jog1.m_translation.y < m_bola.m_translation.y){directionY = 1;}  //tocou no topo
      directionX *= -1;
      m_bola.m_color = glm::vec4(0,0,1,1);
  }
  else if(distXJog2 && distYJog2){ //prox jogador 2
      if(m_jog2.m_translation.y == m_bola.m_translation.y)    {directionY = 0;}
      else if(m_jog2.m_translation.y > m_bola.m_translation.y){directionY = -1;}
      else if(m_jog2.m_translation.y < m_bola.m_translation.y){directionY = 1;}
      directionX *= -1;
      m_bola.m_color = glm::vec4(1,0,0,1);
  }
  else if(abs(m_bola.m_translation.y) >= 1.0f) {directionY *= -1;} //borda lateral
  if(abs(m_bola.m_translation.x) >= 0.98f)     {directionX *= -1;} //borda sup/vert
}

void OpenGLWindow::fezGol() {
  if(m_bola.m_translation.x >= 0.96f && m_bola.m_translation.y <= 0.25f && m_bola.m_translation.y >=-0.25f ){
    Jog1Score +=1;
    directionX = 0;
    if(Jog1Score == 3){
      m_gameData.m_state = State::VitJG1;
      Jog1Score = 0;
      Jog2Score = 0;
    }
    else{
      m_gameData.m_state = State::Gol;
    }
    m_restartWaitTimer.restart();
  }
  else if(m_bola.m_translation.x <= -0.9f && m_bola.m_translation.y <= 0.25f && m_bola.m_translation.y >=-0.25f){
    Jog2Score+=1;
    directionX = 0;
    if(Jog2Score == 3){
      m_gameData.m_state = State::VitJG2;
      Jog1Score = 0;
      Jog2Score = 0;
    }
    else{
      m_gameData.m_state = State::Gol;
    }
    m_restartWaitTimer.restart();
  }

}