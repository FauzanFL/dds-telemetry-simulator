#pragma once
#include <dds/dds.hpp>

namespace QoSProfile {

inline dds::pub::qos::PublisherQos PubQoS(const std::string& partition_name, const std::vector<uint8_t>& node_info) {
    dds::pub::qos::PublisherQos pub_qos;
    dds::core::policy::Presentation presentation_policy;
    presentation_policy.coherent_access(false);
    presentation_policy.access_scope(dds::core::policy::PresentationAccessScopeKind::INSTANCE);
    return pub_qos
           // << dds::core::policy::Partition({partition_name})
           << dds::core::policy::GroupData(node_info)
           << presentation_policy;
}

inline dds::sub::qos::SubscriberQos SubQoS(const std::string& partition_name, const std::vector<uint8_t>& node_info) {
    dds::sub::qos::SubscriberQos sub_qos;
    dds::core::policy::Presentation presentation_policy;
    presentation_policy.coherent_access(false);
    presentation_policy.access_scope(dds::core::policy::PresentationAccessScopeKind::INSTANCE);
    return sub_qos
           // << dds::core::policy::Partition({partition_name})
           << dds::core::policy::GroupData(node_info)
           << presentation_policy;
}

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
    << dds::core::policy::History::KeepLast(10)
    << dds::core::policy::OwnershipStrength(100)
    << dds::core::policy::WriterDataLifecycle::AutoDisposeUnregisteredInstances()
    << dds::core::policy::DataRepresentation({dds::core::policy::DataRepresentationId::XCDR2});
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
    << dds::core::policy::History::KeepLast(1)
    << dds::core::policy::Lifespan(dds::core::Duration::from_secs(2))
    << dds::core::policy::Deadline(dds::core::Duration::from_millisecs(500))
    << dds::core::policy::LatencyBudget(dds::core::Duration::from_millisecs(10))
    << dds::core::policy::DestinationOrder::SourceTimestamp()
    << dds::core::policy::ResourceLimits(100,100,100);
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

inline dds::sub::qos::DataReaderQos ReliableVolatileSub(const dds::sub::Subscriber& sub, int depth = 1) {
    return sub.default_datareader_qos()
    << dds::core::policy::Reliability::Reliable()
    << dds::core::policy::Durability::Volatile()
    << dds::core::policy::History::KeepLast(depth);
}

inline dds::sub::qos::DataReaderQos BestEffortVolatileSub(const dds::sub::Subscriber& sub) {
    return sub.default_datareader_qos()
    << dds::core::policy::Reliability::BestEffort()
    << dds::core::policy::Durability::Volatile()
    << dds::core::policy::History::KeepLast(1)
    << dds::core::policy::LatencyBudget(dds::core::Duration::from_millisecs(10))
    << dds::core::policy::TimeBasedFilter(dds::core::Duration::from_millisecs(100))
    << dds::core::policy::ReaderDataLifecycle(
        dds::core::Duration::from_secs(60),
        dds::core::Duration::from_secs(120))
    << dds::core::policy::TypeConsistencyEnforcement(
           dds::core::policy::TypeConsistencyKind::ALLOW_TYPE_COERCION);
}
}