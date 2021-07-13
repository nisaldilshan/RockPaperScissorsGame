#pragma once

#define DEFAULT_BUFLEN 512

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