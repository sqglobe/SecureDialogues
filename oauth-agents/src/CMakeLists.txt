project(oauth-agents)

include(${TOP_DIR}/secrets.txt)

set(SOURCES 
  oauth-agents/agents/gmailoauth.cpp
  oauth-agents/agents/vkoauth.cpp

  oauth-agents/agents/gmailapi.cpp
  oauth-agents/agents/vkapi.cpp

  oauth-agents/utils/base64.cpp
  oauth-agents/utils/factories.cpp
  oauth-agents/exceptions/oauth-exceptions.cpp
)


set(HEADERS 
  oauth-agents/interfaces/oauthagent.h
  oauth-agents/interfaces/apiagent.h

  oauth-agents/agents/gmailoauth.h
  oauth-agents/agents/vkoauth.h

  oauth-agents/agents/gmailapi.h
  oauth-agents/agents/vkapi.h

  oauth-agents/utils/base64.h
  oauth-agents/utils/factories.h

  oauth-agents/exceptions/oauth-exceptions.h
)

add_library(oauth-agents STATIC ${SOURCES} ${HEADERS})

target_include_directories(oauth-agents PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(oauth-agents PUBLIC curl-wrapper-headers nlohmann_json::nlohmann_json)

add_library(oauth-agents-headers INTERFACE)
target_include_directories(oauth-agents-headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
