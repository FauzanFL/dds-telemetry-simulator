#ifndef PUBLISHERS_H
#define PUBLISHERS_H

#include "ipublisher.h"
#include "MissionRoute.hpp"
#include "RCWSCommand.hpp"
#include "RCWSStatus.hpp"
#include "Telemetry.hpp"
#include "TargetTrack.hpp"

class MissionRoutePub : public IPublisher {
public:
    using IPublisher::IPublisher;
    void run() override;
};

class RCWSCommandPub : public IPublisher {
public:
    using IPublisher::IPublisher;
    void run() override;
};

class RCWSStatusPub : public IPublisher {
public:
    using IPublisher::IPublisher;
    void run() override;
};

class TelemetryPub : public IPublisher {
public:
    using IPublisher::IPublisher;
    void run() override;
};

class TargetTrackPub : public IPublisher {
public:
    using IPublisher::IPublisher;
    void run() override;
};

#endif // PUBLISHERS_H
