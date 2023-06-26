#include "SettingsScene.h"

#include "core/renderer/Renderer.h"
#include "core/Style.h"
#include "core/UI.h"
#include "core/Mixer.h"

namespace ltrm
{

void SettingsScene::Update(float timestep)
{
  UI::Title("Settings");
  
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
