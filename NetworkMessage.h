#pragma once

#define DEFAULT_BUFLEN 512

enum class MessageType { 
    PlayerInput, 
    PlayerPlayAgainInput,
    Ack
}; 

struct NetworkMessage
{
    MessageType type;
    char data[DEFAULT_BUFLEN - sizeof(MessageType)];
};