#ifndef IMAPRECIEVER_H
#define IMAPRECIEVER_H

#include <list>
#include <string>
#include <vmime/vmime.hpp>

class ImapReciever {
 public:
  ImapReciever(const std::string& address,
               int port,
               bool tlsUsed,
               std::string folder,
               const std::string& userName,
               const std::string& pass,
               std::string subject);
  void connect();

  std::list<std::pair<std::string, std::string>> recievedMessages();

 private:
  std::string mFolder;
  std::string mSubject;

  vmime::shared_ptr<vmime::net::folder> mImapFolder;
  vmime::shared_ptr<vmime::net::store> mStore;
  vmime::net::message::uid mLastMessage;
  vmime_uint32 mUidValidity;
};

#endif  // IMAPRECIEVER_H
