#include "menu.h"
#include "ui.h"
#include "renderer.h"
#include "style.h"
#include "animation.h"
#include "input.h"
#include "game.h"
#include "audio.h"

namespace ltrm
{

MenuScene::MenuScene()
{
  Animation titleAnim;
  titleAnim.Min = -300;
  titleAnim.Max = 300;
  titleAnim.Duration = 0.5;
  titleAnim.Type = AnimationType::Wave;
  titleAnim.ResetOnComplete = true;
  titleAnim.Loop = false;
  titleAnim.Active = false;
  m_TitleAnimID = Animator::RegisterAnimation(titleAnim);
}

void MenuScene::Load()
{
  m_LastTime = SDL_GetTicks();
}

void MenuScene::Unload()
{
  
}

void MenuScene::Update(float timestep)
{
  Renderer::Clear(Color::Accent);
  
  float time = SDL_GetTicks();
  m_PulseTime += time - m_LastTime;
  m_LastTime = time;
  if (m_PulseTime >= 3000.0f)
  {
    m_PulseTime = 0.0f;
    UI::PulseTiles();
  }
  
  // TITLE
  {
    if (Input::KeyPress(SDLK_a)) Animator::SetAnimationActive(m_TitleAnimID);
    
    float x = Renderer::GetWidth() / 2 + Animator::QueryAnimation(m_TitleAnimID).Value;
    float y = Style::SmallMargin + Style::TileSize / 2;
    UI::TiledText(std::string("Letter"), x, y, 2);
    UI::TiledText(std::string("morph"), x, y + Style::TileSize + Style::SmallMargin, 2);
  }
  
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
