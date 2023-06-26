#include "MenuScene.h"

#include "core/UI.h"
#include "core/renderer/Renderer.h"
#include "core/Style.h"
#include "core/Animation.h"
#include "core/Input.h"
#include "core/Game.h"
#include "core/Mixer.h"

namespace ltrm
{

void MenuScene::Load()
{
}

void MenuScene::Unload()
{
}

void MenuScene::Update(float timestep)
{
  Renderer::Clear(Color::Accent);
  
  static float pulseTime = 0;
  pulseTime += timestep;
  if (pulseTime >= 3.0f)
  {
    pulseTime = 0;
    UI::PulseTiles();
  }

  float x = Renderer::GetWidth() / 2;
  float y = Style::SmallMargin + Style::TileSize / 2;
  UI::TiledText(std::string("Letter"), x, y, 2);
  UI::TiledText(std::string("morph"), x, y + Style::TileSize + Style::SmallMargin, 2);
  
	constexpr float btnWidth = 800;
  constexpr float btnHeight = 120;
  constexpr float scale = 0.7f;
  constexpr float layoutWidth = btnWidth;
  constexpr float smallBtnWidth = (layoutWidth - Style::SmallMargin) / 2;
  
  float cx = Renderer::GetWidth() / 2;
  float cy = Renderer::GetHeight() / 2;
  float lx = cx - (layoutWidth - smallBtnWidth) / 2;
  float rx = cx + (layoutWidth - smallBtnWidth) / 2;
  
  if (UI::Button("Play", cx, cy - Style::SmallMargin - btnHeight, btnWidth, btnHeight, scale))
    SceneStack::PushScene("selection");
  
  if (UI::Button("Leaderboard", lx, cy, smallBtnWidth, btnHeight, scale))
    SceneStack::PushScene("leaderboard");

  if (UI::Button("Settings", rx, cy, smallBtnWidth, btnHeight, scale))
    SceneStack::PushScene("settings");
  
  if (UI::Button("Help", lx, cy + Style::SmallMargin + btnHeight, smallBtnWidth, btnHeight, scale))
    SceneStack::PushScene("help");
  
  if (UI::Button("Quit", rx, cy + Style::SmallMargin + btnHeight, smallBtnWidth, btnHeight, scale))
    Game::GetInstance()->Stop();
}

void MenuScene::KeyDown(SDL_Keycode key)
{
  Mixer::Pop();
}

}
