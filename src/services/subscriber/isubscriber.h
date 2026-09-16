#ifndef ISUBSCRIBER_H
#define ISUBSCRIBER_H

#include <dds/dds.hpp>

class ISubscriber {
public:
    explicit ISubscriber(dds::domain::DomainParticipant participant) : participant_(participant) {}
    virtual ~ISubscriber() = default;
    virtual void listen() = 0;

protected:
    dds::domain::DomainParticipant participant_;
};

#endif // ISUBSCRIBER_H
