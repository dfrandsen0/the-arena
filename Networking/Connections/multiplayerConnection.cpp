#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <string>

#include "multiplayerConnection.h"
#include "..\..\config.h"

MultiplayerConnection::MultiplayerConnection(SOCKET sock) : Connection() {
    this->serverSocket = sock;
    this->buffer = (char*)malloc(NETWORKING_MULTIPLAYER_BUFFER_SIZE);
    memset(this->buffer, 0, NETWORKING_MULTIPLAYER_BUFFER_SIZE);
}

MultiplayerConnection::~MultiplayerConnection() {
    
}

InfoPacket** MultiplayerConnection::ReceiveInput(int currFrame, int* numPackets) {
    InfoPacket** packetList = new InfoPacket*[NETWORKING_MAX_PACKETS];
    *numPackets = 0;

    int recvSize;
    char tempBuffer[256];
    memset(tempBuffer, 0, 256);

    for(;;) {
        recvSize = recv(this->serverSocket, tempBuffer, 256, 0);

        if(recvSize <= 0) {
            //} && WSAGetLastError() != WSAEWOULDBLOCK) {
            break;
        }

        if(recvSize == 0) {
            std::cout << "Server Failed. Panic!" << std::endl;
            break;
        }

        std::cout << "recvsize: " << recvSize << std::endl;

        memcpy(this->buffer + this->currLength, tempBuffer, recvSize);
        this->currLength += recvSize;
        
        std::string data(this->buffer, this->currLength);
        size_t pos = 0;
        for(;;) {
            // std::cout << "trying to handle input" << std::endl;
            //Handle another player cutting the connection (TERMINATE) TODO

            const std::string startTag = "FRAME";
            const std::string endTag   = "END";

            std::cout << data << std::endl;
            size_t start = data.find(startTag, pos);
            if(start == std::string::npos) {
                std::cout << "no start" << std::endl;
                break;
            }

            size_t end = data.find(endTag, start);
            if(end == std::string::npos) {
                std::cout << "no end" << std::endl;
                break;
            }

            end += endTag.size();


            int type, xParam, yParam, idParam, playerNum, frameCount;

            std::string msg = data.substr(start, end - start);
            int parsed = sscanf(
                msg.c_str(),
                "FRAME %d %d %d %d %d %d END",
                &type, &xParam, &yParam, &idParam, &playerNum, &frameCount
            );

            if(parsed == 6) {
                // std::cout << "creating packet: " << type << " " << xParam << " " << yParam << " " << idParam << " " << playerNum << " " << frameCount << std::endl;
                InfoPacket* newPacket = new InfoPacket(new MainInput(static_cast<MainInputID>(type), xParam, yParam, idParam), frameCount, playerNum);
                packetList[*numPackets] = newPacket;
                (*numPackets)++;
            } else {
                std::cout << "Issue with FRAME packet" << std::endl;
            }

            pos = end;
        }

        if(pos > 0) {
            std::string tail = data.substr(pos);
            memcpy(this->buffer, tail.data(), tail.size());
            this->currLength = (int)tail.size();
            memset(this->buffer + this->currLength, 0, NETWORKING_MULTIPLAYER_BUFFER_SIZE - this->currLength);
        }
    }

    return packetList;
}

void MultiplayerConnection::SendInput(InfoPacket* packet) {
    std::string packetString = "FRAME " +
        std::to_string(static_cast<int>(packet->input->type)) + " " +
        std::to_string(packet->input->xParam) + " " +
        std::to_string(packet->input->yParam) + " " +
        std::to_string(packet->input->idParam) + " " +
        std::to_string(packet->playerNum) + " " +
        std::to_string(packet->frameNum) + " END";

    this->SendOut(packetString);
}

void MultiplayerConnection::BreakConnection() {
    closesocket(this->serverSocket);
    WSACleanup();
}

bool MultiplayerConnection::SendOut(const std::string& msg) {
    int totalSent = 0;
    int toSend = (int)msg.size();
    while (totalSent < toSend) {
        int sent = send(this->serverSocket, msg.c_str() + totalSent, toSend - totalSent, 0);
        if (sent <= 0) return false;
        totalSent += sent;
    }
    return true;
}
