#include "settings.h"
#include "renderer.h"
#include "style.h"
#include "ui.h"
#include "audio.h"

namespace ltrm
{

SettingsScene::SettingsScene()
{
  
}

SettingsScene::~SettingsScene()
{
  
}

void SettingsScene::Load()
{

}

void SettingsScene::Unload()
{
  
}

void SettingsScene::Update()
{
  Renderer::Clear(Color::Accent);
  
  {
    float time = SDL_GetTicks();
    m_PulseTime += time - m_LastTime;
    m_LastTime = time;
    
    if (m_PulseTime >= 3000.0f)
    {
      m_PulseTime = 0.0f;
      UI::PulseTiles();
    }
    
    UI::TiledText(std::string("Settings"), Renderer::GetWidth() / 2, Style::SmallMargin + Style::TileSize / 2, 2);
  }
  
  UI::Text("Music Volume", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2 - 350, 0.8f);
  if (UI::Slider(Renderer::GetWidth() / 2 - 300, Renderer::GetHeight() / 2 - 300, 600, 150, 0, 100, m_MusicVolume))
  {
    Mixer::SetMusicVolume(m_MusicVolume / 100);
  }
  
  UI::Text("Effects Volume", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2, 0.8f);
  if (UI::Slider(Renderer::GetWidth() / 2 - 300, Renderer::GetHeight() / 2 + 50, 600, 150, 0, 100, m_FXVolume))
  {
    Mixer::SetFXVolume(m_FXVolume / 100);
  }
  
  float y = Renderer::GetHeight() / 2 + 2 * (150 + Style::SmallMargin);
  if (UI::Button("Back", Renderer::GetWidth() / 2, y + 100, false))
  {
    SceneManager::ChangeScene("main");
  }
}

void SettingsScene::KeyDown(SDL_Keycode key)
{
  
}

}
