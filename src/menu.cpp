#include "menu.h"
#include "ui.h"
#include "renderer.h"
#include "style.h"
#include "animation.h"
#include "input.h"
#include "game.h"

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
  m_StartTime = SDL_GetTicks();
}

void MenuScene::Unload()
{
  
}

void MenuScene::Update()
{
  float activeTime = SDL_GetTicks() - m_StartTime;
  if (!m_Pulsed && activeTime >= 2000.0f)
  {
    m_Pulsed = true;
    UI::PulseTiles();
  }
  
  Renderer::Clear(Color::Accent);
  
  {
    if (Input::KeyPress(SDLK_a)) Animator::SetAnimationActive(m_TitleAnimID);
    
    float x = Renderer::GetWidth() / 2 + Animator::QueryAnimation(m_TitleAnimID).Value;
    float y = Style::SmallMargin + Style::TileSize / 2;
    UI::TiledText(std::string("Letter"), x, y, 2);
    UI::TiledText(std::string("morph"), x, y + Style::TileSize + Style::SmallMargin, 2);
  }
  
  if (UI::Button("Play", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2, 400, 150, true))
  {
    SceneManager::ChangeScene("selection");
  }
  
  if (UI::Button("Quit", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2 + 150 + Style::SmallMargin, 400, 150, true))
  {
    Game::GetInstance()->Stop();
  }
}

void MenuScene::KeyDown(SDL_Keycode key)
{

}

}
