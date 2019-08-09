#ifndef DIALOGACTIONDELIVERYHANDLER_H
#define DIALOGACTIONDELIVERYHANDLER_H

#include "containers/storages.h"
#include "deliveryhandler.h"

class AbstractMessageDespatcher;
class AbstractUserNotifier;
class CryptoSystemImpl;

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_create_dialog_request(
    std::weak_ptr<AbstractMessageDespatcher> dispatcher,
    DialogStorage::wrapper_type&& wrapper,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::string&& address,
    std::string&& channel);

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_wait_verification_dialog_request(
    std::weak_ptr<AbstractMessageDespatcher> dispatcher,
    DialogStorage::wrapper_type&& wrapper,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::string&& address,
    std::string&& channel);

std::shared_ptr<DeliveryHandler> make_delivery_handler_for_close_dialog_request(
    std::weak_ptr<AbstractMessageDespatcher> dispatcher,
    DialogStorage::wrapper_type&& wrapper,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::string&& address,
    std::string&& channel);

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_active_dialog_request(
    std::weak_ptr<AbstractMessageDespatcher> dispatcher,
    DialogStorage::wrapper_type&& wrapper,
    std::shared_ptr<AbstractUserNotifier> notifier,
    std::string&& address,
    std::string&& channel);

#endif  // DIALOGACTIONDELIVERYHANDLER_H
