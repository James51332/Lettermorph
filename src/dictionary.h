#pragma once

#include "word.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace ltrm
{

// I think we can get away with a binary search to find the
// in the dictionary. I just don't know how crazy the memory usage
// will be. The total dictionary is only about 600kb, so it should
// be totally fine to be allocated on the heap.

class Dictionary
{
public:
  static void Init();
  static void Shutdown();
  
  static bool CheckWord(const char* word);
  static int AlphaCompare(const char* word1, const char* word2);
  
private:
  static std::vector<std::string> s_Words;
};

}
