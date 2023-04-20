#include "RenderAPI.h"

#include <SDL3/SDL.h>
#include <cstring>

namespace ltrm
{

const char* RenderAPI::GetIdentifierFromAPI(RenderAPI::API api)
{
  switch (api)
  {
    case API::OpenGL: return "opengl";
    case API::Metal: return "metal";
    case API::DirectX: return "direct3d12";
    case API::Vulkan: return "vulkan";
    default: SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unknown RenderAPI!");
  }
  
  return nullptr;
}

RenderAPI::API RenderAPI::GetAPIFromIdentifier(const char* identifier)
{
  if (strcmp(identifier,"opengl")) return API::OpenGL;
  if (strcmp(identifier,"metal")) return API::Metal;
  if (strcmp(identifier,"direct3d12")) return API::DirectX;
  if (strcmp(identifier,"vulkan")) return API::Vulkan;
  
  SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unknown RenderAPI Identifier %s!", identifier);
  return API::Unknown;
}

std::vector<RenderAPI::API> RenderAPI::QueryAvaiableAPIs()
{
  std::vector<RenderAPI::API> apis;
  
  int num = SDL_GetNumRenderDrivers();
  for (int i = 0; i < num; i++)
  {
    RenderAPI::API api = GetAPIFromIdentifier(SDL_GetRenderDriver(i));
    
    bool used = false;
    for (auto usedAPI : apis) used |= (api == usedAPI);
    if (used) continue;
    
    apis.push_back(api);
  }
  
  return apis;
}

}
