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
  
  UI::Text("Lettermorph", Renderer::GetWidth() / 2, 200);
  
  if (UI::Button("Play", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2))
  {
    SceneManager::ChangeScene("level");
  }
}

void MenuScene::KeyDown(SDL_Keycode key)
{
  
}

}
