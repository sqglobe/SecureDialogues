project(oauth-agents)

include(${TOP_DIR}/secrets.txt)

set(SOURCES 
  oauth-agents/agents/gmailoauth.cpp
  oauth-agents/agents/vkoauth.cpp

  oauth-agents/agents/gmailapi.cpp
  oauth-agents/agents/vkapi.cpp

  oauth-agents/utils/base64.cpp
  oauth-agents/utils/factories.cpp
)


set(HEADERS 
  oauth-agents/interfaces/oauthagent.h
  oauth-agents/interfaces/apiagent.h

  oauth-agents/agents/gmailoauth.h
  oauth-agents/agents/vkoauth.h

  oauth-agents/agents/gmailapi.h
  oauth-agents/agents/vkapi.h

  oauth-agents/exceptions/httpfailexception.h

  oauth-agents/utils/base64.h

  oauth-agents/utils/factories.h

)


add_library(oauth-agents STATIC ${SOURCES} ${HEADERS})

target_include_directories(oauth-agents PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(oauth-agents PUBLIC cpprestsdk::cpprest boost_system)

