#include "menu.h"
#include "ui.h"
#include "renderer.h"
#include "colors.h"

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
  Renderer::Clear(Color::Accent);
  
  constexpr int width = 500;
  constexpr int height = 300;
  int x = (Renderer::GetWidth() - width) / 2;
  int y = (Renderer::GetHeight() - height) / 2;
  
  if (UI::Button(x, y, width, height))
  {
    SceneManager::ChangeScene("level");
  }
  UI::Text("Play", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2);
  
}

void MenuScene::KeyDown(SDL_Keycode key)
{
  
}

}
