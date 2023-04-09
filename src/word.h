#pragma once

#include <string>

namespace ltrm {

class Word
{
public:
  Word();
  
  size_t GetLength() const { return m_String.length(); }
  const char* GetValue() const { return m_String.c_str(); }
  
  void PushChar(char c) { m_String.push_back(c); }
  void PopChar() { if (GetLength() > 0) m_String.pop_back(); }
  
  const char operator[] (size_t index) const { return m_String[index]; }
  char operator[] (size_t index) { return m_String[index]; }
  
private:
  std::string m_String;
};

}
