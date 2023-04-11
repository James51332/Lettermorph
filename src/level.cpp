#include "level.h"

#include "renderer.h"
#include "dictionary.h"
#include "ui.h"

#include <SDL3/SDL.h>

namespace ltrm
{

LevelScene::LevelScene()
{
  m_TargetWord = new Word("HARP");
  m_Words.push_back(new Word("MILE"));
  m_Words.push_back(new Word(""));
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
    
    x += tileSize + tileMargin;
  }
}

void LevelScene::Update()
{
  Renderer::Clear({0x10, 0x10, 0x10, 0xff});
  
  // Draw working words
  float x = Renderer::GetWidth() / 2;
  float y = tileMargin;
  int num = 0;
  for (auto* word : m_Words)
  {
    DrawWord(word, x, y + num * (tileSize + tileMargin));
    num++;
  }
  
  // Draw target words
  x = Renderer::GetWidth() / 2;
  y = Renderer::GetHeight() - (tileMargin + tileSize);
  DrawWord(m_TargetWord, x, y);
  
  // Win Screen
  if (m_Won)
  {
    if (UI::Button(0, 0, 100, 100))
    {
      SDL_Log("pressed!");
    }
  }
}

void LevelScene::Unload()
{
}

void LevelScene::KeyDown(SDL_Keycode key)
{
  if (m_Won) return;
  
  Word* lastWord = m_Words[m_Words.size()-1];
  if (isalpha(key))
  {
    if (lastWord->GetLength() < m_TargetWord->GetLength())
    	lastWord->PushChar(key);
  }
  else if (key == SDLK_BACKSPACE)
  {
    lastWord->PopChar();
  }
  else if (key == SDLK_RETURN)
  {
    // We need to check if the word is valid
    {
    	if (lastWord->GetLength() != m_TargetWord->GetLength()) return;
      if (!Dictionary::CheckWord(lastWord->GetValue())) return;
    	
      Word* previous = m_Words[m_Words.size() - 2];
    	size_t length = lastWord->GetLength();
    	size_t differences = 0;
    	for (size_t i = 0; i < length; i++)
    	{
    	    char c1 = SDL_toupper(lastWord->GetValue()[i]);
    	    char c2 = SDL_toupper(previous->GetValue()[i]);
    	    if (c1 != c2) differences++;
    	}
      if (differences != 1) return;
    }
    
    // If we won, finished with level!
    if (Dictionary::AlphaCompare(lastWord->GetValue(), m_TargetWord->GetValue()) == 0)
    {
      m_Won = true;
    }
    
    // Append a new word to the back
    m_Words.push_back(new Word(""));
  }
}

}
