#ifndef IPUBLISHER_H
#define IPUBLISHER_H

#include <dds/dds.hpp>

class IPublisher {
public:
    explicit IPublisher(dds::domain::DomainParticipant participant) : participant_(participant) {}
    virtual ~IPublisher() = default;
    virtual void run() = 0;

protected:
    dds::domain::DomainParticipant participant_;
};

#endif // IPUBLISHER_H