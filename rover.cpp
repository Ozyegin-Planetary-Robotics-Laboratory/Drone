#include <iostream>
#include <string>
#include <sstream>
#include <arpa/inet.h>
#include <unistd.h>
#include <tmotor.hpp>
#include <vector>
#include <memory>

int main() {
    int sockfd;
    sockaddr_in rover_addr, client_addr;
    char buffer[1024];

    // Create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return -1;
    }
    std::cout << "Socket created successfully.\n";

    // Setup rover address
    rover_addr.sin_family = AF_INET;
    rover_addr.sin_addr.s_addr = INADDR_ANY;
    rover_addr.sin_port = htons(8888); // Same port as used in the client program

    // Bind socket
    if (bind(sockfd, (const sockaddr*)&rover_addr, sizeof(rover_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        close(sockfd);
        return -1;
    }
    std::cout << "Socket bound successfully to port 8888.\n";

    // Initialize motor managers
    std::vector<int> motor_ids = {0x01, 0x02, 0x03, 0x04}; // Replace with actual motor IDs
    std::vector<std::shared_ptr<TMotor::AKManager>> managers;
    for (int id : motor_ids) {
        try {
            auto manager = std::make_shared<TMotor::AKManager>(id);
            manager->connect("can0");
            std::cout << "Connected to motor ID: " << id << std::endl;
            managers.push_back(manager);
        } catch (const std::exception &e) {
            std::cerr << "Failed to connect to motor ID " << id << ": " << e.what() << std::endl;
            close(sockfd);
            return -1;
        }
    }

    socklen_t len = sizeof(client_addr);
    while (true) {
        // Receive data from client
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&client_addr, &len);
        if (n < 0) {
            std::cerr << "Failed to receive data" << std::endl;
            continue;
        }
        buffer[n] = '\0';
        std::cout << "Received data: " << buffer << std::endl;

        // Parse received data
        std::istringstream iss(buffer);
        float linear_velocity = 0.0f, angular_velocity = 0.0f;
        iss >> linear_velocity >> angular_velocity;
        std::cout << "Parsed linear velocity: " << linear_velocity << ", angular velocity: " << angular_velocity << std::endl;

        // Calculate velocities for each motor
        float front_left_velocity = linear_velocity + angular_velocity;
        float front_right_velocity = linear_velocity - angular_velocity;
        float rear_left_velocity = linear_velocity + angular_velocity;
        float rear_right_velocity = linear_velocity - angular_velocity;

        // Send velocities to motors and log the actions
        try {
            managers[0]->sendVelocity(front_left_velocity);
            std::cout << "Sent front left motor velocity: " << front_left_velocity << std::endl;
            managers[1]->sendVelocity(front_right_velocity);
            std::cout << "Sent front right motor velocity: " << front_right_velocity << std::endl;
            managers[2]->sendVelocity(rear_left_velocity);
            std::cout << "Sent rear left motor velocity: " << rear_left_velocity << std::endl;
            managers[3]->sendVelocity(rear_right_velocity);
            std::cout << "Sent rear right motor velocity: " << rear_right_velocity << std::endl;
        } catch (const std::exception &e) {
            std::cerr << "Error sending motor velocity: " << e.what() << std::endl;
            continue;
        }
    }

    close(sockfd);
    return 0;
}


can0  00002903   [8]  FE FA 00 00 00 25 1B 00
  can0  00002902   [8]  06 D9 00 00 00 1F 1D 00
  can0  00002904   [8]  06 CE 00 00 00 1B 1D 00
  can0  00002901   [8]  FE 99 00 00 00 40 1B 00
  can0  00002903   [8]  FE FA 00 00 00 28 1B 00
  can0  00002902   [8]  06 D9 00 01 00 22 1D 00
  can0  00002904   [8]  06 CE 00 00 00 1F 1D 00
  can0  00002901   [8]  FE 99 00 00 00 3D 1B 00
  can0  00002903   [8]  FE FA 00 00 00 27 1B 00
  can0  00002902   [8]  06 D9 00 00 00 27 1D 00
  can0  00002904   [8]  06 CE 00 00 00 23 1D 00
  can0  00002901   [8]  FE 99 00 00 00 3D 1B 00
  can0  00002903   [8]  FE FA 00 00 00 28 1B 00
  can0  00002902   [8]  06 D9 00 00 00 2B 1D 00
  can0  00000301   [4]  00 00 00 00
  can0  00000302   [4]  00 00 00 15
  can0  00000303   [4]  00 00 00 00
  can0  00000304   [4]  00 00 00 15
  can0  00002904   [8]  06 CE 00 00 00 2A 1D 00
  can0  00002901   [8]  FE 99 00 00 00 3D 1B 00
  can0  00002903   [8]  FE FA 00 00 00 27 1B 00
  can0  00002902   [8]  06 D9 00 00 00 35 1D 00
  can0  00002904   [8]  06 CE 00 00 00 2D 1D 00
  can0  00002901   [8]  FE 99 00 00 00 40 1B 00
  can0  00002903   [8]  FE FA 00 00 00 27 1B 00
  can0  00002902   [8]  06 D9 00 00 00 37 1D 00
  can0  00002904   [8]  06 CE 00 01 00 34 1D 00
  can0  00002901   [8]  FE 99 00 00 00 3E 1B 00
  can0  00002903   [8]  FE FA 00 00 00 2A 1B 00
  can0  00002902   [8]  06 D9 00 00 00 3E 1D 00
  can0  00002904   [8]  06 CE 00 00 00 39 1D 00
  can0  00002901   [8]  FE 99 00 00 00 3D 1B 00
  can0  00002903   [8]  FE FA 00 00 00 26 1B 00
  can0  00002902   [8]  06 D9 00 00 00 45 1D 00
  can0  00002904   [8]  06 CE 00 00 00 3D 1D 00
  can0  00002901   [8]  FE 99 00 00 00 3E 1B 00
  can0  00002903   [8]  FE FA 00 00 00 24 1B 00
  can0  00002902   [8]  06 D9 00 00 00 4A 1D 00
  can0  00000301   [4]  00 00 00 00
  can0  00000302   [4]  00 00 00 00
  can0  00000303   [4]  00 00 00 00
  can0  00000304   [4]  00 00 00 00
  can0  00002904   [8]  06 CE 00 00 00 44 1D 00
  can0  00002901   [8]  FE 99 00 01 00 3E 1B 00
  can0  00002903   [8]  FE FA 00 00 00 28 1B 00
  can0  00002902   [8]  06 D9 00 00 00 51 1D 00
  can0  00002904   [8]  06 CE 00 00 00 43 1D 00
  can0  00002901   [8]  FE 99 00 00 00 3F 1B 00
  can0  00002903   [8]  FE FA 00 00 00 28 1B 00
  can0  00002902   [8]  06 D9 00 00 00 53 1D 00
  can0  00002904   [8]  06 CE 00 00 00 41 1D 00
  can0  00002901   [8]  FE 99 00 00 00 3F 1B 00
^Ckaine@ares:~$ 
