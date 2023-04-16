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

MenuScene::~MenuScene()
{
  
}

void MenuScene::Load()
{
  m_LastTime = SDL_GetTicks();
}

void MenuScene::Unload()
{
  
}

void MenuScene::Update()
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
  
  constexpr float layoutWidth = btnWidth;
  constexpr float smallBtnWidth = (layoutWidth - Style::SmallMargin) / 2;
  
  if (UI::Button("Play", Renderer::GetWidth() / 2, (Renderer::GetHeight() - Style::SmallMargin - btnHeight) / 2, btnWidth, btnHeight, 0.8f))
  {
    SceneManager::ChangeScene("selection");
  }
  
  if (UI::Button("Help", (Renderer::GetWidth() - layoutWidth + smallBtnWidth) / 2, (Renderer::GetHeight() + Style::SmallMargin + btnHeight) / 2, smallBtnWidth, btnHeight, 0.8f))
  {
    SceneManager::ChangeScene("help");
  }
  
  if (UI::Button("Settings", (Renderer::GetWidth() + layoutWidth - smallBtnWidth) / 2, (Renderer::GetHeight() + Style::SmallMargin + btnHeight) / 2, smallBtnWidth, btnHeight, 0.8f))
  {
    SceneManager::ChangeScene("settings");
  }
}

void MenuScene::KeyDown(SDL_Keycode key)
{
  Mixer::Pop();
}

}
