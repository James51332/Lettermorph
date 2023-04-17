#include "entry.h"
#include "ui.h"
#include "renderer.h"
#include "audio.h"
#include "leaderboard.h"
#include "style.h"

namespace ltrm
{

int EntryScene::s_Score = 0;

EntryScene::EntryScene()
{
  
}

EntryScene::~EntryScene()
{
  
}

void EntryScene::Load()
{
  
}

void EntryScene::Unload()
{
  
}

void EntryScene::Update()
{
  Renderer::Clear(Color::Accent);
  
  UI::Text("Enter your name!", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2 - 200, 1.2f);
  float width = UI::TiledTextWidth(m_Word.length() >= 3 ? m_Word.length() : 3);
  UI::TiledText(m_Word, (Renderer::GetWidth() - width) / 2, Renderer::GetHeight() / 2, 1, m_Word.length() >= 3 ? m_Word.length() : 3);
}

void EntryScene::KeyDown(SDL_Keycode key)
{
  if (isalpha(key))
  {
    char c = m_Word.length() == 0 ? SDL_toupper(key) : SDL_tolower(key);
    m_Word.push_back(c);
  	UI::PulseLastTile();
  	Mixer::Pop();
    return;
  }
  
  if (key == SDLK_BACKSPACE)
  {
    if (m_Word.length() == 0) return;
    m_Word.pop_back();
    Mixer::Pop();
  }
  
  if (key == SDLK_RETURN)
  {
    LeaderboardScene::AddEntry(m_Word, s_Score);
    Mixer::Pop();
    SceneManager::ChangeScene("leaderboard");
  }
}

}
