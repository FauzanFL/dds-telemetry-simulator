#include "mastersubscriber.h"
#include "MissionRoute.hpp"
#include "RCWSCommand.hpp"
#include "RCWSStatus.hpp"
#include "Telemetry.hpp"
#include "TargetTrack.hpp"
#include "../../config/qos_profiles.h"

#include <iostream>
#include <thread>
#include <chrono>

void MasterSubscriber::listen() {
    dds::sub::Subscriber sub(participant_);

    // Readers Definition
    dds::sub::DataReader<Ship::MissionRoute> readerRoute(sub, dds::topic::Topic<Ship::MissionRoute>(participant_, "Ship/MissionRoute"), QoSProfile::ReliableTransientLocalSub(sub));
    dds::sub::DataReader<Weapon::RCWSCommand> readerCmd(sub, dds::topic::Topic<Weapon::RCWSCommand>(participant_, "Weapon/RCWSCommand"), QoSProfile::ReliableTransientLocalSub(sub));
    dds::sub::DataReader<Weapon::RCWSStatus> readerStat(sub, dds::topic::Topic<Weapon::RCWSStatus>(participant_, "Weapon/RCWSStatus"), QoSProfile::ReliableTransientLocalSub(sub));
    dds::sub::DataReader<Ship::Telemetry> readerTelem(sub, dds::topic::Topic<Ship::Telemetry>(participant_, "Ship/Telemetry"), QoSProfile::BestEffortVolatileSub(sub));
    dds::sub::DataReader<Tactical::TargetTrack> readerTarget(sub, dds::topic::Topic<Tactical::TargetTrack>(participant_, "Tactical/TargetTrack"), QoSProfile::BestEffortVolatileSub(sub));

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

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    if (inputThread.joinable()) {
        inputThread.join();
    }
    std::cout << "\n[INFO] Master Subscriber dihentikan. Kembali ke menu...\n";
}
