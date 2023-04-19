#include "leaderboard.h"
#include "ui.h"
#include "renderer.h"
#include "style.h"
#include "level.h"

#include <cstring>
#include <iostream>
#include <sstream>

namespace ltrm
{

std::list<LeaderboardScene::Entry> LeaderboardScene::s_Entries;

LeaderboardScene::LeaderboardScene()
{
  m_Leaderboard = SDL_RWFromFile("resources/leaderboard.txt", "r+");
  if (!m_Leaderboard)
  {
    SDL_Log("Failed to open leaderboard: %s", SDL_GetError());
  }
  
  size_t size = SDL_RWsize(m_Leaderboard);
  std::string buf = std::string(size, ' ');
  SDL_RWread(m_Leaderboard, &buf[0], size);
  std::istringstream data(buf);
  
  
  std::string token;
  std::string name;
  int score;
  while (std::getline(data, token))
  {
    bool num = true;
    for (auto c : token) if (!isnumber(c)) num = false;
    
    if (num)
    {
      score = std::stoi(token);
      AddEntry(name, score);
    } else
    {
      name = token;
    }
  }
}

LeaderboardScene::~LeaderboardScene()
{
  SDL_RWseek(m_Leaderboard, 0, 0);
  
  for (auto& entry : s_Entries)
  {
    std::string text;
    text.append(entry.Name);
    text.append("\n");
    text.append(std::to_string(entry.Score));
    text.append("\n");
    
    SDL_RWwrite(m_Leaderboard, text.data(), text.length());
  }
  
  SDL_RWclose(m_Leaderboard);
}

void LeaderboardScene::Load()
{
}

void LeaderboardScene::Unload()
{
}

void LeaderboardScene::Update(float timestep)
{
  m_PulseTime += timestep;
  if (m_PulseTime >= 3.0f)
  {
    m_PulseTime = 0;
    UI::PulseTiles();
  }
  
  Renderer::Clear(Color::Accent);
  UI::TiledText(std::string("LEADERBOARD"), Renderer::GetWidth() / 2, Style::SmallMargin + Style::TileSize / 2, 2);
  
  float y = Renderer::GetHeight() / 2 + 2 * (150 + Style::SmallMargin);
  if (UI::Button("Back", Renderer::GetWidth() / 2, y + 100, false))
  {
    SceneStack::ChangeScene("main");
  }
  
  float panelWidth = 800;
  float panelHeight = 600;
  float panelX = (Renderer::GetWidth() - panelWidth) / 2;
  float panelY = (Renderer::GetHeight() - panelHeight) / 2;
  Renderer::Fill(Color::Dark);
  Renderer::Stroke(Color::Middle);
  Renderer::StrokeWeight(5);
  Renderer::Rect(panelX, panelY, panelWidth, panelHeight);
  
  float textHeight;
  UI::TextSize("Score", nullptr, &textHeight, 1.0f);
  y = panelY + 5 + Style::SmallMargin + textHeight / 2;
  UI::Text("Scores", Renderer::GetWidth() / 2, y, 1.0f);
  
  float x = Renderer::GetWidth() / 2;
  float height;
  UI::TextSize("1.", nullptr, &height, 0.75f);
  
  size_t place = 1;
  y += Style::SmallMargin;
  for (auto& entry : s_Entries)
  {
    y += height + Style::SmallMargin;
    std::string s = std::to_string(place).append(". ").append(entry.Name).append(": ").append(std::to_string(entry.Score));
    UI::Text(s.c_str(), x, y, 0.75f);
    place++;
  }
}

void LeaderboardScene::AddEntry(const std::string &name, int score)
{
  size_t i = 0;
  for (auto& entry : s_Entries)
  {
    if (entry.Name == name)
    {
      entry.Score = (entry.Score < score) ? entry.Score : score;
      return;
    }
    
  	if (entry.Score >= score)
    {
      std::list<Entry>::iterator it = s_Entries.begin();
      std::advance(it, i);
      s_Entries.emplace(it, name, score);
      LevelScene::Reset();
      return;
    }
    i++;
  }
  
  std::list<Entry>::iterator it = s_Entries.begin();
  std::advance(it, i);
  s_Entries.emplace(it, name, score);
}

}
