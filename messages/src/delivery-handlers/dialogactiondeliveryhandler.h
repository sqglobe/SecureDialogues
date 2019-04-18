#ifndef DIALOGACTIONDELIVERYHANDLER_H
#define DIALOGACTIONDELIVERYHANDLER_H

#include "deliveryhandler.h"

#include "containers/dialogmanager.h"

class AbstractMessageDespatcher;
class AbstractUserNotifier;
class CryptoSystemImpl;

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_create_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier);

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_cancel_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier);

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_wait_verification_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier);

std::shared_ptr<DeliveryHandler> make_delivery_handler_for_close_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier);

std::shared_ptr<DeliveryHandler>
make_delivery_handler_for_active_dialog_request(
    const std::weak_ptr<AbstractMessageDespatcher>& dispatcher,
    DialogManager::wrapper_type&& wrapper,
    const std::shared_ptr<AbstractUserNotifier>& notifier);

#endif  // DIALOGACTIONDELIVERYHANDLER_H
