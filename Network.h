#pragma once

constexpr int DEFAULT_BUFLEN = 4096;

constexpr char* DEFAULT_PORT = "27015";
constexpr char* DEFAULT_IP = "127.0.0.1";


enum class MessageType { 
    PlayerInput, 
    PlayerPlayAgainInput,
    Winner,
    GameSummary,
    Ack
}; 

constexpr int SIZEOF_DATA =  DEFAULT_BUFLEN - sizeof(MessageType) - sizeof(int);

struct NetworkMessage
{
    MessageType type;
    int length;
    char data[SIZEOF_DATA];
};