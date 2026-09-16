#ifndef MASTERSUBSCRIBER_H
#define MASTERSUBSCRIBER_H

#include "isubscriber.h"

class MasterSubscriber : public ISubscriber {
public:
    using ISubscriber::ISubscriber;
    void listen() override;
};

#endif // MASTERSUBSCRIBER_H
