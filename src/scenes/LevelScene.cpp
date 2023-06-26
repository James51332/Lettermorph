#include "LevelScene.h"

#include "core/renderer/Renderer.h"
#include "core/Dictionary.h"
#include "core/UI.h"
#include "core/Style.h"
#include "core/Animation.h"
#include "core/Mixer.h"

#include "scenes/EntryScene.h"

#include "Levels.h"

#include <SDL3/SDL.h>
#include <cstring>

namespace ltrm
{

int LevelScene::s_Level = 1;
std::vector<int> LevelScene::s_CompleteScores;

LevelScene::LevelScene()
{
  // Register animations that are used within the scene to the animator
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
  
  // Set all scores to zero.
  for (size_t i = 0; i < numLevels; i++)
    s_CompleteScores.push_back(0);
}

void LevelScene::SetLevel(int level)
{
  SDL_assert(level >= 1 || level <= numLevels);
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
  m_Words.push_back(std::string());
  
  m_ScrollOffset = 0;
  m_MenuIsOpen = false;
}

void LevelScene::Update(float timestep)
{
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
  size_t index = 0;
  for (auto& word : m_Words)
  {
    bool lastWord = (index == m_Words.size() - 1);
    if (lastWord) x += Animator::QueryAnimation(m_ShakeAnimation).Value;
    
    UI::TiledText(word,
                  x,
                  y + index * (Style::TileSize + Style::SmallMargin),
                  lastWord ? 1 : 0, m_WordLength);
    index++;
  }
  
  // Draw target word
  {
  	x = Renderer::GetWidth() / 2;
  	y = (Renderer::GetHeight() / 2 + 750) - (Style::TileSize + Style::SmallMargin);
  	UI::TiledText(m_TargetWord, x, y);
  }
    
  // Draw menu controls
  {
  	constexpr float btnWidth = 200, btnHeight = 100;
  	float width = Renderer::GetWidth();
  	bool shouldOpenMenu = UI::Button("Menu",
  	                                 width - (btnWidth / 2 + Style::SmallMargin), // Button x (center)
  	                                 (btnHeight / 2) + Style::SmallMargin, // Button y (center)
  	                                 btnWidth,
  	                                 btnHeight,
  	                                 Style::SmallScale);
  	
  	if ((shouldOpenMenu || m_MenuIsOpen) && !m_Won) // Don't open the menu if we won.
  	{
  	  m_MenuIsOpen = true;
  	  ShowMenu();
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
      SceneStack::PopScene();
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
    SceneStack::Reload();
  } else
  {
    if (m_AllComplete)
      SceneStack::ChangeScene("entry");
    else
      SceneStack::PopScene();
  }
}

void LevelScene::KeyDown(SDL_Keycode key)
{
  if (m_MenuIsOpen)
  {
    m_MenuIsOpen = false;
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

void LevelScene::ShowMenu()
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
      SceneStack::PushScene("help");
    if (UI::Button("Reset", rx, by, btnWidth, btnHeight, Style::SmallScale))
      SceneStack::Reload();
    if (UI::Button("Home", lx, ty, btnWidth, btnHeight, Style::SmallScale))
      SceneStack::ClearStack();
    if (UI::Button("Settings", rx, ty, btnWidth, btnHeight, Style::SmallScale))
      SceneStack::PushScene("settings");
    
    if (UI::Button("Back", Renderer::GetWidth() / 2, panelY + panelHeight - buttonPadding - btnHeight / 2, btnWidth, btnHeight, Style::SmallScale))
    {
      m_MenuIsOpen = false;
    }
}

}
