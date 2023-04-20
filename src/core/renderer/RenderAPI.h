#pragma once


#include <vector>
namespace ltrm
{

/** Simple enum of possible RenderAPIs that can be supported by application
 */
class RenderAPI
{
public:
  enum class API
  {
    Unknown,
    OpenGL,
    Metal,
    DirectX,
    Vulkan
  };
  
  static const char* GetIdentifierFromAPI(RenderAPI::API api);
  static RenderAPI::API GetAPIFromIdentifier(const char* identifier);
  
  static std::vector<RenderAPI::API> QueryAvaiableAPIs();
};


}

