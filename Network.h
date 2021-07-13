#pragma once

#define DEFAULT_BUFLEN 2048

constexpr char* DEFAULT_PORT = "27015";
constexpr char* DEFAULT_IP = "127.0.0.1";


enum class MessageType { 
    PlayerInput, 
    PlayerPlayAgainInput,
    Winner,
    GameSummary,
    Ack
}; 

#define SIZEOF_DATA  DEFAULT_BUFLEN - sizeof(MessageType) - sizeof(int)

struct NetworkMessage
{
    MessageType type;
    int length;
    char data[SIZEOF_DATA];
};