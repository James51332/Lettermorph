#include "HelpScene.h"

#include "core/UI.h"
#include "core/Style.h"
#include "core/renderer/Renderer.h"

namespace ltrm
{

static constexpr const char* helpText =
R"(The goal of Lettermorph is to connect one word that is given to another by changing only one letter at a time, with each change resulting in a valid English word. For example, if the target is BOT and the start is CAR, the player could go CAR to CAT to COT to BOT.)";

void HelpScene::Load()
{
}

void HelpScene::Unload()
{
  
}

void HelpScene::Update(float timestep)
{
  Renderer::Clear(Color::Accent);
  
  static float pulseTime = 0;
  pulseTime += timestep;
  if (pulseTime >= 3.0f)
  {
    pulseTime = 0;
    UI::PulseTiles();
  }
  UI::TiledText(std::string("Help"), Renderer::GetWidth() / 2, Style::SmallMargin + Style::TileSize / 2, 2);
  
  float y = Renderer::GetHeight() / 2 + 2 * (150 + Style::SmallMargin);
  if (UI::Button("Back", Renderer::GetWidth() / 2, y + 100, false))
    SceneStack::PopScene();
  
  float panelWidth = 800;
  float panelHeight = 600;
  float panelX = (Renderer::GetWidth() - panelWidth) / 2;
  float panelY = (Renderer::GetHeight() - panelHeight) / 2;
  Renderer::Fill(Color::Dark);
  Renderer::Stroke(Color::Middle);
  Renderer::StrokeWeight(5);
  Renderer::Rect(panelX, panelY, panelWidth, panelHeight);
  
  UI::WrappedText(helpText, Renderer::GetWidth()/2, Renderer::GetHeight()/2, panelWidth - 2 * Style::SmallMargin, 0.6f);
}

}
