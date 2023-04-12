#include "menu.h"
#include "ui.h"
#include "renderer.h"
#include "style.h"

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
  
  UI::Text("Lettermorph", Renderer::GetWidth() / 2, 200, 1.0f);
  
  if (UI::Button("Play", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2, true))
  {
    SceneManager::ChangeScene("level");
  }
}

void MenuScene::KeyDown(SDL_Keycode key)
{
  
}

}
