#include "deliveryhandler.h"

void removed(const std::shared_ptr<DeliveryHandler>& obj) {
  obj->removed();
}

void timeouted(const std::shared_ptr<DeliveryHandler>& obj) {
  obj->timeouted();
}
