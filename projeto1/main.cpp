#include <fmt/core.h>

#include "abcg.hpp"
#include "openglwindow.hpp"

int main(int argc, char **argv) {
  try {
    abcg::Application app(argc, argv);

    auto window{std::make_unique<OpenGLWindow>()};
    window->setOpenGLSettings({.samples = 15});

    //.f guarantees no affection
    window->setWindowSettings({.width = 450,
                               .height = 450,
                               .showFPS = false,
                               .showFullscreenButton = false,
                               .title = "Gol a Gol"});
    app.run(std::move(window));
  } catch (abcg::Exception &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}