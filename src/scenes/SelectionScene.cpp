#include "SelectionScene.h"

#include "core/Style.h"
#include "core/renderer/Renderer.h"
#include "core/UI.h"

#include "scenes/LevelScene.h"

#include <string>

namespace ltrm
{

void SelectionScene::Load()
{
}

void SelectionScene::Unload()
{
}

void SelectionScene::Update(float timestep)
{
  Renderer::Clear(Color::Accent);
  
  // Pulse Title
  static float pulseTime = 0;
  pulseTime += timestep;
  if (pulseTime >= 3.0f)
  {
    pulseTime = 0;
    UI::PulseTiles();
  }
  UI::TiledText(std::string("Levels"), Renderer::GetWidth() / 2, Style::SmallMargin + Style::TileSize / 2, 2);
  
	// Draw a grid of level buttons
  constexpr float size = 150;
  constexpr float margin = Style::SmallMargin;
  
  float x = Renderer::GetWidth() / 2;
  float y = Renderer::GetHeight() / 2;
  
  x -= 2 * (size + margin);
  y -= 1 * (size + margin);
  
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      int num = i * 5 + j + 1;
      std::string s = std::to_string(num);
      if (UI::Button(s.c_str(), x, y, size, size, 0.6f))
      {
        LevelScene::SetLevel(num);
        SceneStack::PushScene("level");
      }
      
      x += size + margin;
    }
    y += size + margin;
    x = Renderer::GetWidth() / 2 - 2 * (size + margin);
  }
  
  if(UI::Button("Back", Renderer::GetWidth() / 2, y + 100, false))
    SceneStack::PopScene();
}

} 
