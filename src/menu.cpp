#include "menu.h"
#include "ui.h"
#include "renderer.h"

namespace ltrm
{

MenuScene::MenuScene()
{
  
}

MenuScene::~MenuScene()
{
  
}

void MenuScene::Load()
{
  
}

void MenuScene::Unload()
{
  
}

void MenuScene::Update()
{
  Renderer::Clear({0,0,0,255});
  
  if (UI::Button(0, 0, 400, 400))
  {
    SceneManager::ChangeScene("level");
  }
}

void MenuScene::KeyDown(SDL_Keycode key)
{
  
}

}
