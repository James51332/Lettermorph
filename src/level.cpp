#include "level.h"

#include "renderer.h"
#include "dictionary.h"
#include "ui.h"
#include "style.h"
#include "animation.h"

#include <SDL3/SDL.h>

namespace ltrm
{

LevelScene::LevelScene()
{
  Animation shake;
  shake.Type = AnimationType::Wave;
  shake.Min = -15;
  shake.Max = 15;
  shake.Loop = false;
  shake.Duration = 0.2;
  m_ShakeAnimation = Animator::RegisterAnimation(shake);

  Animation scroll;
  scroll.Type = AnimationType::Lerp;
  scroll.Min = 0;
  scroll.Max = Style::TileSize + Style::SmallMargin;
  scroll.Duration = 0.15;
  scroll.Loop = false;
  m_ScrollAnimation = Animator::RegisterAnimation(scroll);
}

LevelScene::~LevelScene()
{
}

void LevelScene::SetLevel(int level)
{
  
}

void LevelScene::Load()
{
  m_TargetWord = new Word("HARP");
  m_Words.push_back(new Word("MILE"));
  m_Words.push_back(new Word(""));
  
  m_ScrollOffset = 0;
}

static float WordWidth(size_t length)
{
  return length * (Style::TileSize + Style::SmallMargin) - Style::SmallMargin;
}

static void DrawWord(Word* word, float x, float y, size_t size = 0)
{
  if (size == 0)
  {
    x -= WordWidth(word->GetLength()) / 2;
    size = word->GetLength();
  }
  
  for (int i = 0; i < size; i++)
  {
    float centerX = x + Style::TileSize / 2;
    float centerY = y + Style::TileSize / 2;
    
    Renderer::NoStroke();
    Renderer::Fill(Color::Dark);
    Renderer::Rect(x, y, Style::TileSize, Style::TileSize);
    
    if (i < word->GetLength())
    {
      char c = word->operator[](i);
      Renderer::Letter(c, centerX - Style::LetterSize / 2, centerY - Style::LetterSize/2, Style::LetterSize);
    }
    
    x += Style::TileSize + Style::SmallMargin;
  }
}

void LevelScene::Update()
{
  Renderer::Clear(Color::Accent);
  
  // Handle Scroll Animation
  {
    const Animation& anim = Animator::QueryAnimation(m_ScrollAnimation);
    if (anim.Progress == 1)
    {
      m_ScrollOffset += anim.Value;
      Animator::SetAnimationActive(m_ScrollAnimation, false);
      Animator::ResetAnimation(m_ScrollAnimation);
    }
  }
  
  // Draw working words
  float y = Style::SmallMargin - m_ScrollOffset - Animator::QueryAnimation(m_ScrollAnimation).Value;
  float x = (Renderer::GetWidth() - WordWidth(m_TargetWord->GetLength())) / 2;
  int num = 0;
  for (auto* word : m_Words)
  {
    if (num == m_Words.size() - 1) x += Animator::QueryAnimation(m_ShakeAnimation).Value;
    DrawWord(word, x, y + num * (Style::TileSize + Style::SmallMargin), m_TargetWord->GetLength());
    num++;
  }
  
  // Draw target words
  x = Renderer::GetWidth() / 2;
  y = Renderer::GetHeight() - (Style::TileSize + Style::SmallMargin);
  DrawWord(m_TargetWord, x, y);
  
  // Win Screen
  if (m_Won)
  {
    float panelWidth = 800;
    float panelHeight = 600;
    float panelX = (Renderer::GetWidth() - panelWidth) / 2;
    float panelY = (Renderer::GetHeight() - panelHeight) / 2;
    float buttonPadding = Style::SmallMargin;
    
    Renderer::Fill(Color::Dark);
    Renderer::Stroke(Color::Middle);
    Renderer::StrokeWeight(5);
    Renderer::Rect(panelX, panelY, panelWidth, panelHeight);
    
    float textHeight;
    UI::TextSize("You Won!", nullptr, &textHeight, 1.0f);
    UI::Text("You Won!", Renderer::GetWidth() / 2, panelY + 100 + textHeight / 2, 1.0f);
    
    float btnWidth, btnHeight;
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
    bool valid = true;
    
    // We need to check if the word is valid
    {
    	if (lastWord->GetLength() != m_TargetWord->GetLength()) valid = false;
      if (!Dictionary::CheckWord(lastWord->GetValue())) valid = false;
    	
      Word* previous = m_Words[m_Words.size() - 2];
    	size_t length = lastWord->GetLength();
    	size_t differences = 0;
    	for (size_t i = 0; i < length; i++)
    	{
    	    char c1 = SDL_toupper(lastWord->GetValue()[i]);
    	    char c2 = SDL_toupper(previous->GetValue()[i]);
    	    if (c1 != c2) differences++;
    	}
      if (differences != 1) valid = false;
    }
    
    if (!valid)
    {
      Animator::SetAnimationActive(m_ShakeAnimation);
      return;
    }
    
    // If we won, finished with level!
    if (Dictionary::AlphaCompare(lastWord->GetValue(), m_TargetWord->GetValue()) == 0)
    {
      m_Won = true;
      return;
    }
    
    // Append a new word to the back
    m_Words.push_back(new Word(""));
    
    if (m_Words.size() > 5)
    {
      Animator::SetAnimationActive(m_ScrollAnimation);
    }
  }
}

}
