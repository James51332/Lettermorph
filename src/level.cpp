#include "level.h"

#include "renderer.h"
#include "dictionary.h"
#include "ui.h"
#include "style.h"
#include "animation.h"
#include "audio.h"
#include "entry.h"

#include <SDL3/SDL.h>
#include <cstring>

namespace ltrm
{

// 25 Levels and Solutions (not necessarily optimal/only)

// I came up with most of these by coming up with a starting
// and ending word and solving. Then I ranked by difficulty.

static constexpr size_t numLevels = 15;
static constexpr const char* levels[] = {
  // EASY
  "CAT-BAR", // CAT -> CAR -> BAR
  "RATE-LAME", // RATE -> LATE -> LAME
  "PANT-CART", // PANT -> PART -> CART
  "SPAT-STAR", // STAR -> STAT -> SPAT
  "MAGIC-PANIC", // MAGIC -> MANIC -> PANIC
  
  // MEDIUM
  "MILE-HARE", // MILE -> MALE -> MARE -> HARE -> HARP
  "LOOP-CREW", // CREW -> CROW -> CROP -> COOP -> LOOP
  "SPACE-SHORT", // SPACE -> SPARE -> SPORE -> SHORE -> SHORT
  "FINE-RICE", // RICE -> RACE -> LACE -> LINE -> FINE
  "WILD-TANK", // TANK -> TALK -> TALL -> WALL -> WILL -> WILD
  
  // VERY HARD
  "PACE-DARE", // DARE -> DATE -> MATE -> MACE -> PACE
  "GRID-WAND", // WAND -> WANT -> WAIT -> WRIT -> GRIT -> GRID
  "PANE-PAIN", // PAIN -> PAID -> LAID -> LAND -> LANE -> PANE
  "PAIR-LEAD", // PAIR -> HAIR -> HEIR -> HEAR -> HEAD -> LEAD -> BEAR
  "STEEP-SPEAK", // SPEAK -> SPEAR -> SHEAR -> SHEER -> SHEEP -> STEEP
};

int LevelScene::s_Level = 1;
std::vector<int> LevelScene::s_CompleteScores;

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
  
  for (size_t i = 0; i < numLevels; i++)
    s_CompleteScores.push_back(0);
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
  m_Menu = false;
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
  float y = (Renderer::GetHeight() / 2 - 750) - m_ScrollOffset - Animator::QueryAnimation(m_ScrollAnimation).Value;
  float x = (Renderer::GetWidth() - UI::TiledTextWidth(m_WordLength)) / 2;
  int num = 0;
  for (auto& word : m_Words)
  {
    if (num == m_Words.size() - 1) x += Animator::QueryAnimation(m_ShakeAnimation).Value;
    UI::TiledText(word, x, y + num * (Style::TileSize + Style::SmallMargin), num == m_Words.size() - 1 ? 1 : 0, m_WordLength);
    num++;
  }
  
  // Draw target words
  x = Renderer::GetWidth() / 2;
  y = (Renderer::GetHeight() / 2 + 750) - (Style::TileSize + Style::SmallMargin);
  UI::TiledText(m_TargetWord, x, y);
  
  m_Menu = UI::Button("Menu", Renderer::GetWidth() - (100 + Style::SmallMargin), 50 + Style::SmallMargin, 200, 100, Style::SmallScale) || m_Menu;
  if (m_Menu && !m_Won)
  {
    float panelWidth = 1000;
    float panelHeight = 800;
    float panelX = (Renderer::GetWidth() - panelWidth) / 2;
    float panelY = (Renderer::GetHeight() - panelHeight) / 2;
    float buttonPadding = Style::SmallMargin;
    
    Renderer::Fill(Color::Dark);
    Renderer::Stroke(Color::Middle);
    Renderer::StrokeWeight(5);
    Renderer::Rect(panelX, panelY, panelWidth, panelHeight);
    
    float textHeight;
    UI::TextSize("Menu", nullptr, &textHeight, 1.2f);
    UI::Text("Menu", Renderer::GetWidth() / 2, panelY + 100 + textHeight / 2, 1.0f);
    
    
    float btnWidth = 200, btnHeight = 100;
    float cx = Renderer::GetWidth() / 2;
    float cy = Renderer::GetHeight() / 2;
    float lx = cx - (Style::SmallMargin + btnWidth) / 2;
    float rx = cx + (Style::SmallMargin + btnWidth) / 2;
    float ty = cy - (Style::SmallMargin + btnHeight) / 2;
    float by = cy + (Style::SmallMargin + btnHeight) / 2;
    
    if (UI::Button("Help", lx, by, btnWidth, btnHeight, Style::SmallScale))
      SceneManager::CoverScene("help");
    if (UI::Button("Reset", rx, by, btnWidth, btnHeight, Style::SmallScale))
      SceneManager::ChangeScene("level");
    if (UI::Button("Home", lx, ty, btnWidth, btnHeight, Style::SmallScale))
      SceneManager::ChangeScene("main");
    if (UI::Button("Settings", rx, ty, btnWidth, btnHeight, Style::SmallScale))
      SceneManager::CoverScene("settings");
    
    if (UI::Button("Back", Renderer::GetWidth() / 2, panelY + panelHeight - buttonPadding - btnHeight / 2, btnWidth, btnHeight, Style::SmallScale))
    {
      m_Menu = false;
    }
  }
  
  // Win Screen
  if (m_Won)
  {
    float panelWidth = 1000;
    float panelHeight = 800;
    float panelX = (Renderer::GetWidth() - panelWidth) / 2;
    float panelY = (Renderer::GetHeight() - panelHeight) / 2;
    float buttonPadding = Style::SmallMargin;
    
    Renderer::Fill(Color::Dark);
    Renderer::Stroke(Color::Middle);
    Renderer::StrokeWeight(5);
    Renderer::Rect(panelX, panelY, panelWidth, panelHeight);
    
    float textHeight;
    UI::TextSize("Level Complete!", nullptr, &textHeight, 1.2f);
    UI::Text("Level Complete!", Renderer::GetWidth() / 2, panelY + 100 + textHeight / 2, 1.0f);
    
    std::string score = "Score: ";
    score.append(std::to_string(m_Words.size()));
    UI::Text(score.c_str(), Renderer::GetWidth() / 2, panelY + 200 + textHeight / 2, 0.7f);
    
    float btnWidth = 200, btnHeight = 100;
    if (UI::Button("Back", Renderer::GetWidth() / 2 - Style::SmallMargin / 2 - btnWidth / 2, panelY + panelHeight - buttonPadding - btnHeight / 2, btnWidth, btnHeight, Style::SmallScale))
    {
      SceneManager::ChangeScene("selection");
    }
    
    if (UI::Button("Next", Renderer::GetWidth() / 2 + Style::SmallMargin / 2 + btnWidth / 2, panelY + panelHeight - buttonPadding - btnHeight / 2, btnWidth, btnHeight, Style::SmallScale))
    {
      NextLevel();
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

void LevelScene::NextLevel()
{
  if (s_Level < numLevels)
  {
    s_Level++;
    SceneManager::ChangeScene("level");
  } else
  {
    if (m_AllComplete)
      SceneManager::ChangeScene("entry");
    else
      SceneManager::ChangeScene("selection");
  }
}

void LevelScene::KeyDown(SDL_Keycode key)
{
  if (m_Menu)
  {
    m_Menu = false;
    Mixer::Pop();
    return;
  }
  
  if (m_Won)
  {
    if (key == SDLK_RETURN)
    {
      NextLevel();
      Mixer::Pop();
    }
    return;
  }
  
  std::string& lastWord = m_Words[m_Words.size() -1];
  if (isalpha(key))
  {
    if (lastWord.length() < m_WordLength)
    {
      lastWord.push_back(key);
      UI::PulseLastTile();
      Mixer::Pop();
    }
  }
  else if (key == SDLK_BACKSPACE)
  {
    if (lastWord.length() > 0)
    {
      lastWord.pop_back();
      Mixer::Pop();
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
      UI::PulseTiles(0.25f);
      Mixer::Win();
      m_Won = true;
      s_CompleteScores[s_Level - 1] = static_cast<int>(m_Words.size());
      
      m_AllComplete = true;
      size_t score = 0;
      for (auto s : s_CompleteScores)
      {
        if (s == 0) m_AllComplete = false;
        score += s;
      }
      if (m_AllComplete) EntryScene::SetScore(static_cast<int>(score));
      
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

void LevelScene::Reset()
{
  for (auto& s : s_CompleteScores)
    s = 0;
}

}
