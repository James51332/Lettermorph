#include "EntryScene.h"

#include "core/UI.h"
#include "core/renderer/Renderer.h"
#include "core/Mixer.h"
#include "core/Style.h"

#include "scenes/LeaderboardScene.h"

namespace ltrm
{

int EntryScene::s_Score = 0;

void EntryScene::Update(float timestep)
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
    SceneStack::ChangeScene("leaderboard");
  }
}

}
