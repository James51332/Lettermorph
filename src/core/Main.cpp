#include "core/Game.h"

int main()
{
  ltrm::Game* game = new ltrm::Game();
  game->Run();
  delete game;
}
