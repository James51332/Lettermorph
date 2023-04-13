#include "selection.h"
#include "style.h"
#include "renderer.h"
#include "ui.h"

#include <string>

namespace ltrm
{

SelectionScene::SelectionScene()
{
  
}

SelectionScene::~SelectionScene()
{
  
}

void SelectionScene::Load()
{
  
}

void SelectionScene::Unload()
{
  
}

void SelectionScene::Update()
{
  Renderer::Clear(Color::Accent);
  
	// Draw a grid of level buttons
  constexpr float size = 150;
  constexpr float margin = Style::SmallMargin;
  
  float x = Renderer::GetWidth() / 2;
  float y = Renderer::GetHeight() / 2;
  
  x -= 2 * (size + margin);
  y -= 2 * (size + margin);
  
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      int num = i * 5 + j + 1;
      std::string s = std::to_string(num);
      UI::Button(s.c_str(), x, y, size, size, 0.6f);
      
      x += size + margin;
    }
    y += size + margin;
    x = Renderer::GetWidth() / 2 - 2 * (size + margin);
  }
  
  if(UI::Button("Back", Renderer::GetWidth() / 2, y + 100, false))
  {
    SceneManager::ChangeScene("main");
  }
}

void SelectionScene::KeyDown(SDL_Keycode key)
{
  
}

} 
