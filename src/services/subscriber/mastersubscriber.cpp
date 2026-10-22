#include "mastersubscriber.h"
#include "MissionRoute.hpp"
#include "RCWSCommand.hpp"
#include "RCWSStatus.hpp"
#include "Telemetry.hpp"
#include "TargetTrack.hpp"
#include "WeaponControl.hpp"
#include "ShipNavigation.hpp"
#include "TacticalTracks.hpp"
#include "../../config/qos_profiles.h"

#include <iostream>
#include <thread>
#include <chrono>

void MasterSubscriber::listen() {
    // std::vector<uint8_t> node_info = {'P', 'O', 'R', 'T'};
    dds::sub::Subscriber sub(participant_);

    // Readers Definition
    dds::sub::DataReader<Ship::MissionRoute> readerRoute(sub, dds::topic::Topic<Ship::MissionRoute>(participant_, "Ship/MissionRoute"), QoSProfile::ReliableTransientLocalSub(sub));
    dds::sub::DataReader<Weapon::RCWSCommand> readerCmd(sub, dds::topic::Topic<Weapon::RCWSCommand>(participant_, "Weapon/RCWSCommand"), QoSProfile::ReliableTransientLocalSub(sub));
    dds::sub::DataReader<Weapon::RCWSStatus> readerStat(sub, dds::topic::Topic<Weapon::RCWSStatus>(participant_, "Weapon/RCWSStatus"), QoSProfile::ReliableVolatileSub(sub));
    dds::sub::DataReader<Ship::Telemetry> readerTelem(sub, dds::topic::Topic<Ship::Telemetry>(participant_, "Ship/Telemetry"), QoSProfile::BestEffortVolatileSub(sub));
    dds::sub::DataReader<Tactical::TargetTrack> readerTarget(sub, dds::topic::Topic<Tactical::TargetTrack>(participant_, "Tactical/TargetTrack"), QoSProfile::BestEffortVolatileSub(sub));

    dds::sub::DataReader<Niskala::Navigation::OwnshipTelemetry> readerOwnTelemery(sub, dds::topic::Topic<Niskala::Navigation::OwnshipTelemetry>(participant_, "Niskala/Navigation/OwnShipTelemetry"), QoSProfile::telemetryProfile());
    dds::sub::DataReader<Niskala::Tactical::TacticalTrack> readerTacticalTarget(sub, dds::topic::Topic<Niskala::Tactical::TacticalTrack>(participant_, "Niskala/Tactical/TacticalTrack"), QoSProfile::telemetryProfile());
    dds::sub::DataReader<Niskala::Weapon::RCWSStatus> readerRcwsStatus(sub, dds::topic::Topic<Niskala::Weapon::RCWSStatus>(participant_, "Niskala/Weapon/RCWSStatus"), QoSProfile::telemetryProfile());

    std::cout << "\n==================================================\n";
    std::cout << "   MASTER SUBSCRIBER AKTIF - MENDENGARKAN TOPIC   \n";
    std::cout << ">>> Tekan [ENTER] kapan saja untuk kembali ke Menu <<<\n";
    std::cout << "==================================================\n\n";

    std::atomic<bool> running{true};

    std::thread inputThread([&running]() {
        std::cin.get();
        running = false;
    });

    while (running) {
        // Read MissionRoute
        auto samplesRoute = readerRoute.take();
        for (const auto& sample : samplesRoute) {
            if (sample.info().valid()) {
                const auto& data = sample.data();
                std::cout << "[TOPIC: MissionRoute] ShipID: " << data.ship_id()
                          << " | Waypoint: " << data.waypoint()
                          << " | Lat: " << data.latitude()
                          << " | Lon: " << data.longitude() << "\n";
            }
        }

        // Read RCWSCommand
        auto samplesCmd = readerCmd.take();
        for (const auto& sample : samplesCmd) {
            if (sample.info().valid()) {
                const auto& data = sample.data();
                std::cout << "[TOPIC: RCWSCommand] ShipID: " << data.ship_id()
                          << " | RcwsID: " << data.rcws_id()
                          << " | CmdID: " << data.command_id()
                          << " | Azimuth: " << data.azimuth()
                          << " | Elevation: " << data.elevation()
                          << " | Range: " << data.range() << "\n";
            }
        }

        // Read RCWSStatus
        auto samplesStat = readerStat.take();
        for (const auto& sample : samplesStat) {
            if (sample.info().valid()) {
                const auto& data = sample.data();
                std::cout << "[TOPIC: RCWSStatus] ShipID: " << data.ship_id()
                          << " | RcwsID: " << data.rcws_id()
                          << " | State: " << data.state()
                          << " | Ammo: " << data.ammo() << "\n";
            }
        }

        // Read Telemetry
        auto samplesTelem = readerTelem.take();
        for (const auto& sample : samplesTelem) {
            if (sample.info().valid()) {
                const auto& data = sample.data();
                std::cout << "[TOPIC: Telemetry] ShipID: " << data.ship_id()
                          << " | Speed: " << data.speed() << " knots"
                          << " | Heading: " << data.heading() << " deg"
                          << " | Lat: " << data.latitude()
                          << " | Lon: " << data.longitude() << "\n";
            }
        }

        // Read TargetTrack
        auto samplesTarget = readerTarget.take();
        for (const auto& sample : samplesTarget) {
            if (sample.info().valid()) {
                const auto& data = sample.data();
                std::cout << "[TOPIC: TargetTrack]"
                          << " | Distance: " << data.distance() << " m"
                          << " | Bearing: " << data.bearing() << " deg"
                          << " | Lat: " << data.latitude()
                          << " | Lon: " << data.longitude() << "\n";
            }
        }

        // OwnshipTelemetry
        auto samplesOwnTele = readerOwnTelemery.take();
        for (const auto& sample : samplesOwnTele) {
            if (sample.info().valid()) {
                const auto& data = sample.data();
                std::cout << "[TOPIC: Niskala/Navigation/OwnShipTelemetry]"
                          << " | ShipID: " << data.ship_id()
                          << " | Lat: " << data.latitude()
                          << " | Lon: " << data.longitude()
                          << " | Hdg: " << data.heading_deg()
                          << " | Spd: " << data.speed_knots()
                          << " | Roll: " << data.roll_deg()
                          << " | Pitch: " << data.pitch_deg()
                          << " | Timestamp: " << data.timestamp_us() << "\n";
            }
        }

        // Tactical Target
        auto samplesTactical = readerTacticalTarget.take();
        for (const auto& sample : samplesTactical) {
            if (sample.info().valid()) {
                const auto& data = sample.data();
                std::cout << "[TOPIC: Niskala/Tactical/TacticalTarget]"
                          << " | TrackID: " << data.track_id()
                          << " | Lat: " << data.latitude()
                          << " | Lon: " << data.longitude()
                          << " | Spd: " << data.speed_knots()
                          << " | Crs: " << data.course_deg()
                          << " | Identity: " << data.identity_classification()
                          << " | Timestamp: " << data.timestamp_us() << "\n";
            }
        }

        // RCWS Status
        auto samplesRcwsStat = readerRcwsStatus.take();
        for (const auto& sample : samplesRcwsStat) {
            if (sample.info().valid()) {
                const auto& data = sample.data();
                std::cout << "[TOPIC: Niskala/Weapon/RCWSStatus]"
                          << " | ShipID: " << data.ship_id()
                          << " | Azm: " << data.current_azimuth()
                          << " | Ele: " << data.current_elevation()
                          << " | Ammo: " << data.ammo_count()
                          << " | Sys: " << data.system_state()
                          << " | Itr: " << data.interlock_ok()
                          << " | Timestamp: " << data.timestamp_us() << "\n";
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    if (inputThread.joinable()) {
        inputThread.join();
    }
    std::cout << "\n[INFO] Master Subscriber dihentikan. Kembali ke menu...\n";
}
