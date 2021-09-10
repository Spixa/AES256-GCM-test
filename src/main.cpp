
#include <SFML/Network.hpp>
#include "cryptography.h"
#define d(x) std::cout<<x<<std::endl;
int main(int argc, char** argv) 
{
    Cryptography* c = new Cryptography();
    const std::vector<uint8_t> key = Botan::hex_decode("2B7E151628AEDAA2A6ABF7158809CFCC4F3CAD4BC3ABCDACFD53672ABCD3AB5C");
    std::string answer;
    if (argc < 2) {
        std::cout << "Choose role: (c: client/ s: server) ";
        std::getline(std::cin, answer);
    }
    else {
        answer = argv[1][1];
    }
    

    if (answer == "c") {
        sf::TcpSocket* client = new sf::TcpSocket();
        if(client->connect("127.0.0.1", 6565) != sf::Socket::Done) {
            std::cout << "Failed to establish connection.\n";
        }
        while (true) {
            std::string to_send;
            std::cout << "input: ";
            std::getline(std::cin, to_send);

            std::string send =  c->encrypt(to_send, "2B7E151628AEDAA2A6ABF7158809CFCC4F3CAD4BC3ABCDACFD53672ABCD3AB5C");
                
        
            if (client->send(send.c_str(), send.length() + 1) != sf::Socket::Done) {
                std::cout << "Couldn't send data\n";
            }
        }

    } else
    if (answer == "s") {
        sf::TcpListener* listener = new sf::TcpListener();
        listener->listen(6565);

        sf::TcpSocket* new_client = new sf::TcpSocket();
        if (listener->accept(*new_client) == sf::Socket::Done) {
                std::cout << "Unregistered " + new_client->getRemoteAddress().toString() + ":" << new_client->getRemotePort() << " was accepted\n";
        }
        else {
            printf("Server failed to accept new sockets\n\trestart the server\n");
            delete(new_client);
        }  
        int sum;
        char received_data[4096]; size_t received_bytes;
        while (true) {
            memset(received_data, 0, sizeof(received_data));
            if (new_client->receive(received_data, sizeof(received_data), received_bytes) == sf::Socket::Done) {        


                if (received_bytes == 0) {
                    std::cout << "Received empty bytes.\n";
                } else {

                    std::unordered_map<std::string,std::string> cipher;

                    std::cout << "\n\nNew encrypted ongoing: " << received_bytes << "B\n";
                    cipher = c->decrypt(std::string(received_data), "2B7E151628AEDAA2A6ABF7158809CFCC4F3CAD4BC3ABCDACFD53672ABCD3AB5C");

                    std::cout << cipher["iv"] << ' ' << cipher["msg"] << std::endl;
                    auto msg =  hex_decode(cipher["msg"]);
                    std::cout << "msg: ";
                    for (auto x : msg) {
                        std::cout << x;
                    }
                    d("");

                }
            }
        }
    } else
    {
        std::cout << "bad input: neither c nor s\n";
        return -1;
    }

    std::cout <<std::endl;

    

    char pause;
    std::cin>>pause;
    return 0;
}