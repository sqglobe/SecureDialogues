project(messages)

find_package (Threads)
find_package(Boost  REQUIRED system)

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
containers/contactcontainer.cpp 
containers/dialogmanager.cpp 
containers/connectioninfocontainer.cpp 
adapters/udpadapter.cpp
containers/connectcontainerwatcher.cpp 
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
builders/builders.h 
builders/filebuilders.h 
builders/file_builder/filebuilder.h
builders/file_builder/dialogsavestrategy.h
builders/file_builder/contactsavestrategy.h
utils.h 
interfaces/changewatcher.h 
interfaces/cryptosystem.h
communication/dialogactionhandler.h
builders/file_builder/connectionsavestrategy.h
builders/file_builder/connectionsavestrategy.cpp
builders/file_builder/contactsavestrategy.cpp
builders/file_builder/dialogsavestrategy.cpp
interfaces/abstractuserask.h 
containers/basecontainer.h 
containers/contactcontainer.h 
containers/dialogmanager.h 
containers/connectioninfocontainer.h 
containers/containerelementwrapper.h
adapters/udpadapter.h 
containers/connectcontainerwatcher.h 
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
containers/transparentcontainerelementwrapper.h
crypto/cryptosystemimpl.h
crypto/cryptosystemcontactupdateinformator.h
crypto/cryptosystemdialogremoveinformator.h
constants.h
adapters/oauthadapter.h
)


add_library(messages STATIC ${SOURCES} ${HEADERS})

target_include_directories(messages PUBLIC   ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries( messages cryptopp-static xg encryption oauth-agents ${CMAKE_THREAD_LIBS_INIT} ${Boost_SYSTEM_LIBRARY})

