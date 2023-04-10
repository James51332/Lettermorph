#include "level.h"

#include "renderer.h"
#include "dictionary.h"

namespace ltrm
{

LevelScene::LevelScene()
{
  m_TargetWord = new Word("HARP");
  m_Words.push_back(new Word("MILE"));
  m_Words.push_back(new Word("TILE"));
}

LevelScene::~LevelScene()
{
  for (auto* word : m_Words)
    delete word;
  
  delete m_TargetWord;
}

void LevelScene::Load()
{
}

static constexpr float tileSize = 200;
static constexpr float letterSize = 90;
static constexpr float tileMargin = 20;

static void DrawWord(Word* word, float x, float y)
{
  float wordWidth = word->GetLength() * (tileSize + tileMargin) - tileMargin;
  x -= wordWidth / 2;
  for (int i = 0; i < word->GetLength(); i++)
  {
    char c = word->operator[](i);
    float centerX = x + tileSize/2;
    float centerY = y + tileSize/2;
    
    Renderer::Fill({0x55,0x55,0x55,0xff});
    Renderer::Rect(x, y, tileSize, tileSize);
    Renderer::Letter(c, centerX - letterSize/2, centerY-letterSize/2, letterSize);
    
    x+= tileSize + tileMargin;
  }
}

void LevelScene::Update()
{
  Renderer::Clear({0x10, 0x10, 0x10, 0xff});
  
  float x = Renderer::GetWidth() / 2;
  float y = tileMargin;
  int num = 0;
  for (auto* word : m_Words)
  {
    DrawWord(word, x, y + num * (tileSize + tileMargin));
    num++;
  }
}

void LevelScene::Unload()
{
}

void LevelScene::KeyDown(SDL_Keycode key)
{
  if (isalpha(key))
    m_Words[m_Words.size()-1]->PushChar(key);
  else if (key == SDLK_BACKSPACE)
    m_Words[m_Words.size()-1]->PopChar();
  else if (key == SDLK_RETURN)
    m_Words.push_back(new Word(""));
}

}
