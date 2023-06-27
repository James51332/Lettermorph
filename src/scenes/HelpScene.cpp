#include "HelpScene.h"

#include "core/UI.h"
#include "core/Style.h"
#include "core/renderer/Renderer.h"
#include "core/Scene.h"

#include "LevelScene.h"

namespace ltrm
{

static constexpr const char* helpText =
R"(Lettermorph is a word themed game. In each level, your goal is to connect one word to another, by only changing one letter at a time. The catch? Each time you make a change, you must still have a real word. Let's see an example!)";

void HelpScene::Update(float timestep)
{
  UI::Title("Tutorial");
  
  if (UI::InfoPanel("Help", helpText, "Next"))
  {
    LevelScene::SetLevel(0);
    SceneStack::ChangeScene("level");
  }
  
  float y = Renderer::GetHeight() / 2 + 2 * (150 + Style::SmallMargin);
  if (UI::Button("Back", Renderer::GetWidth() / 2, y + 100, false))
    SceneStack::PopScene();
}

}
