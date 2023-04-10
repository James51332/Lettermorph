#include "dictionary.h"

#include <SDL3/SDL.h>
#include <cstdio>
#include <cstring>

namespace ltrm
{

// NOTE: This code really shouldn't be run server side, and the
// changing of the text file could potentially lead to a security
// issue.

// Returns
// 0 if word1 and word2 are equal
// -1 if word1 comes before word2
// 1 if word2 comes before word1
static int alphaCmp(const char* word1, const char* word2)
{
  unsigned long length1 = strlen(word1);
  unsigned long length2 = strlen(word2);
  unsigned long length = length1 < length2 ? length1 : length2;
  
  for (int i = 0; i < length; i++)
  {
    char c1 = SDL_toupper(word1[i]);
    char c2 = SDL_toupper(word2[i]);
    if (c1 == c2) continue;
    return c1 < c2 ? -1 : 1;
  }
  
  if (length1 == length2)
    return 0;
  else if (length1 > length2)
    return 1;
  else
    return -1;
}

std::vector<std::string> Dictionary::s_Words;

void Dictionary::Init()
{
	constexpr const char* dictionary = "resources/dictionary.txt";
  constexpr const char* delim = "\n";
  
  // Load the dictionary into a string
  std::fstream filestream(dictionary);
  if (!filestream)
  {
    SDL_Log("Failed to load dictionary!");
    return;
  }
  
  std::string data;
  filestream.seekg(0, filestream.end);
  data.resize(filestream.tellg());
  filestream.seekg(0, filestream.beg);
  filestream.read(&data[0], data.size());
  filestream.close();
  
  // Break the file into tokens
  char* token = strtok(&data[0], delim);
  while (token != nullptr)
  {
    // Convert each word from dictionary to lower case
    for (int i = 0; i < strlen(token); ++i)
    {
      char word = toupper(token[i]);
      token[i] = word;
    }
    // Push word to end of vector
    std::string s(token);
    s.pop_back();
    s_Words.push_back(s);
    // Get the next word
    token = strtok(nullptr, delim);
  }
}

void Dictionary::Shutdown()
{
}

bool Dictionary::CheckWord(const char *word)
{
  // Employ binary search to check if word is in the dictionary
  float searchRange = s_Words.size() / 2;
  size_t index = searchRange;
  
  std::string dictWord = s_Words[index];
  for (;;)
  {
    int direction = alphaCmp(word, dictWord.c_str());
    if (direction == 0) return true; // If the words are equal
    
    searchRange /= 2;
    if (searchRange < 1) break; // If the words next to us weren't it.
      
    index += static_cast<int>(static_cast<float>(direction) * searchRange);
    dictWord = s_Words[index];
  }
  
  return false;
}

}
