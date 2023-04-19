#include "SettingsScene.h"

#include "core/Renderer.h"
#include "core/Style.h"
#include "core/UI.h"
#include "core/Mixer.h"

namespace ltrm
{

void SettingsScene::Load()
{
}

void SettingsScene::Unload()
{
}

void SettingsScene::Update(float timestep)
{
  Renderer::Clear(Color::Accent);
  
  // If more than three seconds on the scene have passed, we should pulse and reset wait
  m_PulseTime += timestep;
  if (m_PulseTime >= 3.0f)
  {
    m_PulseTime = 0.0f;
  	UI::PulseTiles();
  }
  UI::TiledText(std::string("Settings"), Renderer::GetWidth() / 2, Style::SmallMargin + Style::TileSize / 2, 2);
  
  UI::Text("Music Volume", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2 - 350, 0.8f);
  if (UI::Slider(Renderer::GetWidth() / 2 - 300, Renderer::GetHeight() / 2 - 300, 600, 150, 0, 100, m_MusicVolume))
    Mixer::SetMusicVolume(m_MusicVolume / 100);
  
  UI::Text("Effects Volume", Renderer::GetWidth() / 2, Renderer::GetHeight() / 2, 0.8f);
  if (UI::Slider(Renderer::GetWidth() / 2 - 300, Renderer::GetHeight() / 2 + 50, 600, 150, 0, 100, m_FXVolume))
    Mixer::SetFXVolume(m_FXVolume / 100);
  
  float y = Renderer::GetHeight() / 2 + 2 * (150 + Style::SmallMargin);
  if (UI::Button("Back", Renderer::GetWidth() / 2, y + 100, false))
    SceneStack::PopScene();
}

}
