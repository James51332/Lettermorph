#pragma once

#include <SDL3/SDL.h>

namespace ltrm
{

/** Input State Manager for the application to query.
 */
class Input
{
  friend class Window;
public:
  static void Init();
  static void Update();
  static void Shutdown();

  static float GetMouseX() { return s_MouseX; }
  static float GetMouseY() { return s_MouseY; }
  
  static bool MousePress(int btn) { return s_MouseButtons[btn] && !s_MouseButtonsLast[btn]; }
  static bool MouseDown(int btn) { return s_MouseButtons[btn]; }
  static bool MouseRelease(int btn) { return !s_MouseButtons[btn] && s_MouseButtonsLast[btn]; }
  
  static bool KeyPress(SDL_Keycode key) { return s_Keys[key] && !s_KeysLast[key]; }
  static bool KeyDown(SDL_Keycode key) { return s_Keys[key]; }
  static bool KeyRelease(SDL_Keycode key) { return !s_Keys[key] && s_KeysLast[key]; }
  
private:
  static void SetMousePos(float x, float y);
  static void SetMouseDown(SDL_MouseID btn);
  static void SetMouseUp(SDL_MouseID btn);
  
  static void SetKeyDown(SDL_Keycode key);
  static void SetKeyUp(SDL_Keycode key);
  
private:
  static float s_MouseX, s_MouseY;
  
  constexpr static size_t s_NumButtons = 8;
  static bool s_MouseButtons[s_NumButtons];
  static bool s_MouseButtonsLast[s_NumButtons];
  
  static bool s_Keys[SDL_NUM_SCANCODES];
  static bool s_KeysLast[SDL_NUM_SCANCODES];
};

}
