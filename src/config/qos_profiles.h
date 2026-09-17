#pragma once
#include <dds/dds.hpp>

namespace QoSProfile {

// --- GRUP 1: RELIABLE & TRANSIENT_LOCAL / VOLATILE ---
inline dds::pub::qos::DataWriterQos MissionRoutePub(const dds::pub::Publisher& pub) {
    return pub.default_datawriter_qos()
    << dds::core::policy::Reliability::Reliable()
    << dds::core::policy::Durability::TransientLocal()
    << dds::core::policy::History::KeepLast(5);
}

inline dds::pub::qos::DataWriterQos RCWSCommandPub(const dds::pub::Publisher& pub) {
    return pub.default_datawriter_qos()
    << dds::core::policy::Reliability::Reliable()
    << dds::core::policy::Durability::TransientLocal()
    << dds::core::policy::History::KeepLast(10);
}

inline dds::pub::qos::DataWriterQos RCWSStatusPub(const dds::pub::Publisher& pub) {
    return pub.default_datawriter_qos()
    << dds::core::policy::Reliability::Reliable()
    << dds::core::policy::Durability::Volatile()
    << dds::core::policy::History::KeepLast(1);
}

// --- GRUP 2: BEST_EFFORT & VOLATILE ---
inline dds::pub::qos::DataWriterQos TelemetryPub(const dds::pub::Publisher& pub) {
    return pub.default_datawriter_qos()
    << dds::core::policy::Reliability::BestEffort()
    << dds::core::policy::Durability::Volatile()
    << dds::core::policy::History::KeepLast(1);
}

inline dds::pub::qos::DataWriterQos TargetTrackPub(const dds::pub::Publisher& pub) {
    return pub.default_datawriter_qos()
    << dds::core::policy::Reliability::BestEffort()
    << dds::core::policy::Durability::Volatile()
    << dds::core::policy::History::KeepLast(1);
}

// --- SUBSCRIBER QOS PROFILES ---
inline dds::sub::qos::DataReaderQos ReliableTransientLocalSub(const dds::sub::Subscriber& sub, int depth = 10) {
    return sub.default_datareader_qos()
    << dds::core::policy::Reliability::Reliable()
    << dds::core::policy::Durability::TransientLocal()
    << dds::core::policy::History::KeepLast(depth);
}

inline dds::sub::qos::DataReaderQos BestEffortVolatileSub(const dds::sub::Subscriber& sub) {
    return sub.default_datareader_qos()
    << dds::core::policy::Reliability::BestEffort()
    << dds::core::policy::Durability::Volatile()
    << dds::core::policy::History::KeepLast(1);
}
}