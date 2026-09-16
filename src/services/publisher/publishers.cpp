#include "publishers.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <random>

// Grup 1: Reliable & TransientLocal (Manual Input)
void MissionRoutePub::run() {
    dds::topic::Topic<Ship::MissionRoute> topic(participant_, "Ship/MissionRoute");
    dds::pub::Publisher publisher(participant_);
    dds::pub::qos::DataWriterQos qos = publisher.default_datawriter_qos()
                                       << dds::core::policy::Reliability::Reliable()
                                       << dds::core::policy::Durability::TransientLocal();
    dds::pub::DataWriter<Ship::MissionRoute> writer(dds::pub::Publisher(participant_), topic, qos);

    Ship::MissionRoute data;
    std::cout << "\n--- Input MissionRoute ---\n";
    std::cout << "ShipID: "; std::cin >> data.ship_id();
    std::cout << "Waypoint: "; std::cin >> data.waypoint();
    std::cout << "Latitude: "; std::cin >> data.latitude();
    std::cout << "Longitude: "; std::cin >> data.longitude();

    writer.write(data);
    std::cout << "[SUCCESS] Data MissionRoute terkirim!\n";
}

void RCWSCommandPub::run() {
    dds::topic::Topic<Weapon::RCWSCommand> topic(participant_, "Weapon/RCWSCommand");
    dds::pub::Publisher publisher(participant_);
    dds::pub::qos::DataWriterQos qos = publisher.default_datawriter_qos()
                                       << dds::core::policy::Reliability::Reliable()
                                       << dds::core::policy::Durability::TransientLocal();
    dds::pub::DataWriter<Weapon::RCWSCommand> writer(dds::pub::Publisher(participant_), topic, qos);

    Weapon::RCWSCommand data;
    std::cout << "\n--- Input RCWSCommand ---\n";
    std::cout << "Ship ID: "; std::cin >> data.ship_id();
    std::cout << "RCWS ID: "; std::cin >> data.rcws_id();
    std::cout << "Command ID: "; std::cin >> data.command_id();
    std::cout << "Azimuth: "; std::cin >> data.azimuth();
    std::cout << "Elevation: "; std::cin >> data.elevation();
    std::cout << "Range: "; std::cin >> data.range();

    writer.write(data);
    std::cout << "[SUCCESS] Data RCWSCommand terkirim!\n";
}

void RCWSStatusPub::run() {
    dds::topic::Topic<Weapon::RCWSStatus> topic(participant_, "Weapon/RCWSStatus");
    dds::pub::Publisher publisher(participant_);
    dds::pub::qos::DataWriterQos qos = publisher.default_datawriter_qos()
                                       << dds::core::policy::Reliability::Reliable()
                                       << dds::core::policy::Durability::TransientLocal();
    dds::pub::DataWriter<Weapon::RCWSStatus> writer(dds::pub::Publisher(participant_), topic, qos);

    Weapon::RCWSStatus data;
    std::cout << "\n--- Input RCWSStatus ---\n";
    std::cout << "Ship ID: "; std::cin >> data.ship_id();
    std::cout << "RCWS ID: "; std::cin >> data.rcws_id();
    std::cout << "State: "; std::cin >> data.state();
    std::cout << "Ammo: "; std::cin >> data.ammo();

    writer.write(data);
    std::cout << "[SUCCESS] Data RCWSStatus terkirim!\n";
}

// Grup 2: BestEffort & Volatile (Random Setiap 3 Detik)
void TelemetryPub::run() {
    dds::topic::Topic<Ship::Telemetry> topic(participant_, "Ship/Telemetry");
    dds::pub::Publisher publisher(participant_);
    dds::pub::qos::DataWriterQos qos = publisher.default_datawriter_qos()
                                       << dds::core::policy::Reliability::BestEffort()
                                       << dds::core::policy::Durability::Volatile();
    dds::pub::DataWriter<Ship::Telemetry> writer(dds::pub::Publisher(participant_), topic, qos);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> speedDist(10.0, 40.0);
    std::uniform_real_distribution<double> headDist(0.0, 360.0);

    const char* env_ship_id = std::getenv("SHIP_ID");
    std::string ship_id = (env_ship_id != nullptr) ? std::string(env_ship_id) : "SHIP-DEFAULT";

    std::cout << "\n[INFO] Memulai transmisi Telemetry Data otomatis (tiap 3 detik). Tekan Ctrl+C untuk berhenti.\n";
    std::cout << ">>> Tekan [ENTER] kapan saja untuk kembali ke Menu Utama <<<\n\n";

    std::atomic<bool> running{true};

    // Thread pendengar tombol ENTER
    std::thread inputThread([&running]() {
        std::cin.get();
        running = false;
    });

    while (running) {
        Ship::Telemetry data(ship_id, speedDist(rng), headDist(rng));
        writer.write(data);
        std::cout << "[PUBLISH Telemetry] ShipID: " << data.ship_id()
                  << " | Speed: " << data.speed() << " knots | Heading: " << data.heading() << " deg\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));

        // Cek flag per 100ms agar responsif saat user menekan ENTER
        for (int i = 0; i < 30 && running; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    if (inputThread.joinable()) {
        inputThread.join();
    }
    std::cout << "\n[INFO] Menghentikan simulasi Telemetry Data...\n";
}

void TargetTrackPub::run() {
    dds::topic::Topic<Tactical::TargetTrack> topic(participant_, "Tactical/TargetTrack");
    dds::pub::Publisher publisher(participant_);
    dds::pub::qos::DataWriterQos qos = publisher.default_datawriter_qos()
                                       << dds::core::policy::Reliability::BestEffort()
                                       << dds::core::policy::Durability::Volatile();
    dds::pub::DataWriter<Tactical::TargetTrack> writer(dds::pub::Publisher(participant_), topic, qos);

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<double> distDist(100.0, 5000.0);
    std::uniform_real_distribution<double> bearDist(0.0, 360.0);
    std::uniform_real_distribution<double> latDist(-7.5, -6.5);
    std::uniform_real_distribution<double> lonDist(110.0, 111.0);

    std::cout << "\n[INFO] Memulai transmisi TargetTrack otomatis (tiap 3 detik).\n";
    std::cout << ">>> Tekan [ENTER] kapan saja untuk kembali ke Menu Utama <<<\n\n";

    std::atomic<bool> running{true};

    // Thread pendengar tombol ENTER
    std::thread inputThread([&running]() {
        std::cin.get();
        running = false;
    });
    while (true) {
        Tactical::TargetTrack data(distDist(rng), bearDist(rng), latDist(rng), lonDist(rng));
        writer.write(data);
        std::cout << "[PUBLISH TargetTrack]"
                  << " | Distance: " << data.distance() << " m | Bearing: " << data.bearing()
                  << " deg | Latitude: " << data.latitude() << " | Longitude: " << data.longitude() << "\n";
        for (int i = 0; i < 30 && running; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    if (inputThread.joinable()) {
        inputThread.join();
    }
    std::cout << "\n[INFO] Menghentikan simulasi TargetTrack...\n";
}
