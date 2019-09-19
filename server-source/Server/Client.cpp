#include "Client.h"

#include <assert.h>

Client::Client(IpAndPort ip_and_port) :
    ip_and_port(ip_and_port)
{
    //ctor
}

Client::~Client()
{
    //dtor
}

sf::Vector2i Client::getNextRequestedChunk()
{
    chunk_requests_mutex.lock();

    assert(!chunk_requests.empty());

    sf::Vector2i chunk_request = chunk_requests.front();
    chunk_requests.pop();

    chunk_requests_mutex.unlock();

    return chunk_request;
}

void Client::addRequestedChunk(sf::Vector2i chunk)
{
    chunk_requests_mutex.lock();
    chunk_requests.push(chunk);
    chunk_requests_mutex.unlock();
}

