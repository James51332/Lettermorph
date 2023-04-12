#include "level.h"

#include "renderer.h"
#include "dictionary.h"
#include "ui.h"
#include "style.h"

#include <SDL3/SDL.h>

namespace ltrm
{

LevelScene::LevelScene()
{
}

LevelScene::~LevelScene()
{
}

void LevelScene::Load()
{
  m_TargetWord = new Word("HARP");
  m_Words.push_back(new Word("MILE"));
  m_Words.push_back(new Word(""));
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
    
    Renderer::Fill(Color::Dark);
    Renderer::Rect(x, y, tileSize, tileSize);
    Renderer::Letter(c, centerX - letterSize/2, centerY-letterSize/2, letterSize);
    
    x += tileSize + tileMargin;
  }
}

void LevelScene::Update()
{
  Renderer::Clear(Color::Accent);
  
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
    float panelWidth = 800;
    float panelHeight = 600;
    float panelX = (Renderer::GetWidth() - panelWidth) / 2;
    float panelY = (Renderer::GetHeight() - panelHeight) / 2;
    float buttonPadding = tileMargin;
    
    Renderer::Fill(Color::Dark);
    Renderer::Stroke(Color::Middle);
    Renderer::StrokeWeight(5);
    Renderer::Rect(panelX, panelY, panelWidth, panelHeight);
    
    int textHeight;
    UI::TextSize("You Won!", nullptr, &textHeight, 1.0f);
    UI::Text("You Won!", Renderer::GetWidth() / 2, panelY + 100 + textHeight / 2, 1.0f);
    
    int btnWidth, btnHeight;
    UI::ButtonSize("Back", &btnWidth, &btnHeight, false);
    if (UI::Button("Back", Renderer::GetWidth() / 2, panelY + panelHeight - buttonPadding - btnHeight / 2, false))
    {
      SceneManager::ChangeScene("main");
    }
  }
}

void LevelScene::Unload()
{
  while (!m_Words.empty())
  {
    Word* last = m_Words[m_Words.size() - 1];
    m_Words.pop_back();
    delete last;
  }
  delete m_TargetWord;
  m_Won = false;
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
