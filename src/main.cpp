#include <iostream>
#include <memory>
#include <limits>
#include <dds/dds.hpp>
#include "services/publisher/publishers.h"
#include "services/subscriber/mastersubscriber.h"

// Fungsi pembantu untuk membersihkan buffer input saat terjadi error
void clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main() {
    try {
        dds::domain::DomainParticipant participant(0);

        while (true) {
            std::cout << "\n=================================\n";
            std::cout << "      CYCLONEDDS SIMULATOR      \n";
            std::cout << "=================================\n";
            std::cout << "1. Subscriber (Listen All Topics)\n";
            std::cout << "2. Publisher\n";
            std::cout << "0. Exit\n";
            std::cout << "Pilih Mode: ";

            int mainChoice;
            if (!(std::cin >> mainChoice)) {
                std::cout << "[ERROR] Input tidak valid! Masukkan angka.\n";
                clearInputStream();
                continue;
            }

            if (mainChoice == 0) break;

            if (mainChoice == 1) {
                MasterSubscriber subscriber(participant);
                subscriber.listen();
            } else if (mainChoice == 2) {
                std::cout << "\n--- PILIH TOPIK PUBLISHER ---\n";
                std::cout << "1. Mission Route (Manual Input)\n";
                std::cout << "2. RCWS Command (Manual Input)\n";
                std::cout << "3. RCWS Status (Manual Input)\n";
                std::cout << "4. Telemetry Data (Auto 3s)\n";
                std::cout << "5. Target Data (Auto 3s)\n";
                std::cout << "0. Kembali\n";
                std::cout << "Pilihan: ";

                int pubChoice;
                if (!(std::cin >> pubChoice)) {
                    std::cout << "[ERROR] Input tidak valid! Masukkan angka.\n";
                    clearInputStream();
                    continue;
                }

                if (pubChoice == 0) continue;

                std::unique_ptr<IPublisher> publisher;

                switch (pubChoice) {
                case 1: publisher = std::make_unique<MissionRoutePub>(participant); break;
                case 2: publisher = std::make_unique<RCWSCommandPub>(participant); break;
                case 3: publisher = std::make_unique<RCWSStatusPub>(participant); break;
                case 4: publisher = std::make_unique<TelemetryPub>(participant); break;
                case 5: publisher = std::make_unique<TargetTrackPub>(participant); break;
                default:
                    std::cout << "[ERROR] Pilihan menu tidak tersedia.\n";
                    continue;
                }

                if (publisher) {
                    publisher->run();
                }
            } else {
                std::cout << "[ERROR] Pilihan menu tidak tersedia.\n";
            }
        }
    } catch (const dds::core::Exception& e) {
        std::cerr << "DDS Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}