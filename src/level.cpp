#include "level.h"

#include "renderer.h"
#include "dictionary.h"
#include "ui.h"
#include "style.h"
#include "animation.h"

#include <SDL3/SDL.h>
#include <cstring>

namespace ltrm
{

// 25 Levels and Solutions (not necessarily optimal/only)
static constexpr size_t numLevels = 25;
static constexpr const char* levels[] = {
  "CAT-BAR", // CAT -> CAR -> BAR
  "RATE-LAME", // RATE -> LATE -> LAME
  "RATE-LAME", // RATE -> LATE -> LAME
  "RATE-LAME", // RATE -> LATE -> LAME
  "RATE-LAME", // RATE -> LATE -> LAME
  "RATE-LAME",
  "MAGIC-PANIC", // MAGIC -> MANIC -> PANIC
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "MILE-HARE", // MILE -> MALE -> MARE -> HARE -> HARP
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "SPACE-SHORT", // SPACE -> SPARE -> SPORE -> SHORE -> SHORT
  "BRIM-TICK",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME",
  "RATE-LAME"
};

int LevelScene::s_Level = 1;

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
  if (level < 1 || level > numLevels)
  {
    SDL_Log("Level exceeds max range! (%d)", level);
    return;
  }
  
  s_Level = level;
}

void LevelScene::Load()
{
  std::string levelString = levels[s_Level - 1];
  const char* delim = "-";
  
  char* token = strtok(&levelString[0], delim);
  m_TargetWord = std::string(token);
  m_WordLength = m_TargetWord.length();
  
  token = strtok(nullptr, delim);
  while (token != nullptr)
  {
    m_Words.push_back(std::string(token));
    token = strtok(nullptr, delim);
  }
  m_Words.push_back(std::string(""));
  
  m_ScrollOffset = 0;
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
  float x = (Renderer::GetWidth() - UI::TiledTextWidth(m_WordLength)) / 2;
  int num = 0;
  for (auto& word : m_Words)
  {
    if (num == m_Words.size() - 1) x += Animator::QueryAnimation(m_ShakeAnimation).Value;
    UI::TiledText(word, x, y + num * (Style::TileSize + Style::SmallMargin), m_WordLength, num == m_Words.size() - 1);
    num++;
  }
  
  // Draw target words
  x = Renderer::GetWidth() / 2;
  y = Renderer::GetHeight() - (Style::TileSize + Style::SmallMargin);
  UI::TiledText(m_TargetWord, x, y);
  
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
    
    float btnWidth = 200, btnHeight = 100;
    if (UI::Button("Back", Renderer::GetWidth() / 2 - Style::SmallMargin / 2 - btnWidth / 2, panelY + panelHeight - buttonPadding - btnHeight / 2, btnWidth, btnHeight, Style::SmallScale))
    {
      SceneManager::ChangeScene("main");
    }
    
    if (UI::Button("Next", Renderer::GetWidth() / 2 + Style::SmallMargin / 2 + btnWidth / 2, panelY + panelHeight - buttonPadding - btnHeight / 2, btnWidth, btnHeight, Style::SmallScale))
    {
      if (s_Level < numLevels) s_Level++;
      SceneManager::ChangeScene("level");
    }
  }
}

void LevelScene::Unload()
{
  while (!m_Words.empty())
    m_Words.pop_back();
  
  m_TargetWord = std::string("");
  
  m_Won = false;
}

void LevelScene::KeyDown(SDL_Keycode key)
{
  if (m_Won) return;
  
  std::string& lastWord = m_Words[m_Words.size() -1];
  if (isalpha(key))
  {
    if (lastWord.length() < m_WordLength)
    {
      lastWord.push_back(key);
      UI::PulseLastTile();
    }
  }
  else if (key == SDLK_BACKSPACE)
  {
    if (lastWord.length() > 0)
    {
      lastWord.pop_back();
    }
  }
  else if (key == SDLK_RETURN)
  {
    // We need to check if the word is valid
    bool valid = true;    
    {
    	if (lastWord.length() != m_WordLength) valid = false;
      if (!Dictionary::CheckWord(lastWord.c_str())) valid = false;
    	
      std::string previous = m_Words[m_Words.size() - 2];
    	size_t length = lastWord.length();
    	size_t differences = 0;
    	for (size_t i = 0; i < length; i++)
    	{
    	    char c1 = SDL_toupper(lastWord[i]);
    	    char c2 = SDL_toupper(previous[i]);
    	    if (c1 != c2) differences++;
    	}
      if (differences != 1) valid = false;
    }
    
    if (!valid)
    {
      Animator::SetAnimationActive(m_ShakeAnimation);
      return;
    }
    
    // If we won, finished with level
    if (Dictionary::AlphaCompare(lastWord, m_TargetWord) == 0)
    {
      m_Won = true;
      return;
    }
    
    // Append a new word to the back
    m_Words.push_back(std::string(""));
    
    // TODO: Adjust to window size
    if (m_Words.size() > 5)
    {
      Animator::SetAnimationActive(m_ScrollAnimation);
    }
  }
}

}
