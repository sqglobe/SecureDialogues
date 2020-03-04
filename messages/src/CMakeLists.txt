project(messages)

find_package (Threads)
find_package(persistent-storage REQUIRED)

set(SOURCES 
primitives/dialogmessage.cpp
communication/messagedespatcher.cpp
primitives/contact.cpp
primitives/dialog.cpp
primitives/connectionholder.cpp
interfaces/abstractchanneladapter.cpp 
containers/messagecontainer.cpp
communication/messageactionhandler.cpp
communication/channel.cpp
communication/dialogactionhandler.cpp
utils.cpp 
adapters/udpadapter.cpp
utils/fileutils.cpp
utils/messagemarshaller.cpp
delivery-handlers/deliveryhandler.cpp
delivery-handlers/dialogactiondeliveryhandler.cpp
containers/usermessage.cpp
utils/systemmessagegenerator.cpp
primitives/dialogstatemachine.cpp
crypto/cryptosystemimpl.cpp
crypto/cryptosystemcontactupdateinformator.cpp
crypto/cryptosystemdialogremoveinformator.cpp
adapters/oauthadapter.cpp
adapters/emailadapter.cpp
persistent_storage/contactmarshaller.cpp
persistent_storage/dialogmarshaller.cpp
persistent_storage/connectionmarshaller.cpp
containers/storages.cpp
communication/communication_helpers.cpp
containers/connectstoragelistener.cpp

core-initializer/chaneladapterfactory.cpp
core-initializer/coreinitializer.cpp
primitives/discoveredcontact.cpp
persistent_storage/discoveredcontactmarshaller.cpp
communication/discovercontactagent.cpp
communication/despatcherrorssink.cpp
adapters/pluginadapter.cpp
)


set(HEADERS 
primitives/dialogmessage.h
interfaces/abstractchanneladapter.h 
primitives/connectionholder.h
communication/channel.h
interfaces/abstractmessagehandler.h 
communication/messagedespatcher.h
primitives/contact.h
primitives/dialog.h
interfaces/abstractusernotifier.h 
containers/messagecontainer.h
interfaces/messageconteinereventhandler.h 
communication/messageactionhandler.h
interfaces/abstractmessagedespatcher.h 
utils.h 
interfaces/cryptosystem.h
communication/dialogactionhandler.h
interfaces/abstractuserask.h 
adapters/udpadapter.h 
utils/fileutils.h 
exception/receiveexception.h 
exception/sendexception.h 
utils/closer.h
utils/timeoutedrepository.h
interfaces/abstractmessagemarshaller.h
utils/messagemarshaller.h
delivery-handlers/deliveryhandler.h
delivery-handlers/dialogactiondeliveryhandler.h
containers/usermessage.h
utils/systemmessagegenerator.h
primitives/dialogstatemachine.h
crypto/cryptosystemimpl.h
crypto/cryptosystemcontactupdateinformator.h
crypto/cryptosystemdialogremoveinformator.h
constants.h
adapters/oauthadapter.h
exception/notauthorizedexception.h
exception/disconectedexception.h
adapters/emailadapter.h
persistent_storage/contactmarshaller.h
persistent_storage/dialogmarshaller.h
persistent_storage/connectionmarshaller.h
persistent_storage/store_helpers.h
containers/storages.h
communication/communication_helpers.h
containers/eventwatchers.h
interfaces/changelistener.h
containers/connectstoragelistener.h
interfaces/abstractcoreinitializer.h

core-initializer/chaneladapterfactory.h
core-initializer/coreinitializer.h
primitives/discoveredcontact.h
persistent_storage/discoveredcontactmarshaller.h
communication/discovercontactagent.h
interfaces/abstractdespatcherrorssink.h
communication/despatcherrorssink.h
adapters/pluginadapter.h
)

add_library(messages-headers INTERFACE)
target_include_directories(messages-headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${PERSISTENT-STORAGE_INCLUDE_DIRS} ${CROSSGUID_INCLUDE})
target_link_libraries(messages-headers INTERFACE fmt::fmt-header-only curl-wrapper-headers oauth-agents-headers encryption-headers email-agent-headers utils-headers plugins-support-headers )

add_library(messages STATIC ${SOURCES} ${HEADERS})

target_compile_definitions(messages PRIVATE NO_MEMBER_FUNCTION_PARTIAL_SPECIALIZATION)
target_link_libraries(messages PUBLIC messages-headers)

add_internationals(messages ru_RU.UTF-8 "locale" "${LOCALE_FILES_DIR}" securedialogues 1.1)


