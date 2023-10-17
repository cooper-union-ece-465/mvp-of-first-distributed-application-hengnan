#include <boost/asio.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <boost/array.hpp>
#include <cctype>
using boost::asio::ip::tcp;
#define BOOST_TEST_MODULE <server.cpp>
class server {
public:
    int currentState(tcp::socket &socket);
    int sendReceiveStuff(tcp::socket &socket);
    int execute();
    void handle_accept(const boost::system::error_code& error, tcp::socket& socket);
    server(int port) : state("sleep"), port(port) {}
    
    ~server() {}
    int getPort(){ return port;}
    void setState(std::string newState) {state = newState;}
    std::string getState() {return state;}
private:
    
    std::string state;
    int port;
};

int server::currentState(tcp::socket &socket) {
    
    for(;;){
        boost::array<char, 1024> buffer;
        boost::system::error_code error;
        size_t length = socket.read_some(boost::asio::buffer(buffer), error);
        if(error == boost::asio::error::eof)
            break;
        else if(error)
            throw boost::system::system_error(error);
        std::string message(buffer.data(),length);
        setState(message);
        for (char &c : message) {
            c = std::toupper(c);
        }
        
        if (message == "UPLOAD") {
            setState(message);
            sendReceiveStuff(socket);
        } else if (message == "DOWNLOAD") {
            setState(message);
            sendReceiveStuff(socket);
        } else if (message == "QUIT") {
            sendReceiveStuff(socket);
            break;
        }
    }
    return 0;
}

int server::sendReceiveStuff(tcp::socket &socket) {
    int choice;
    if(getState() == "UPLOAD"){choice = 1;}
    else if(getState() == "DOWNLOAD"){choice = 2;}
    else{choice = 3;}
    switch (choice) {
      case 1:
        {
            std::cout << "Client is uploading to Server" << std::endl;
            std::ofstream outputFile("uploaded_file.txt", std::ios::binary);
            boost::array<char, 1024> buffer;
            size_t totalBytes = 0;
            
            while (true) {
                boost::system::error_code error;
                size_t length = socket.read_some(boost::asio::buffer(buffer), error);
                if (error == boost::asio::error::eof)
                    break;
                else if (error)
                    throw boost::system::system_error(error);
                if (std::string(buffer.data(), length) == "end")
                {
                    std::cout << "Received 'end' from the client. Ending upload." << std::endl;
                    break;
                }

                totalBytes += length;
                outputFile.write(buffer.data(), length);
                std::cout << "Received: " << length << " Bytes, current total: " << totalBytes << std::endl;
            }
            
            std::cout << "upload completed" << std::endl;
            outputFile.close();
            setState("sleep");
            break;
        }
        case 2:
        {
            try {
                boost::array<char, 1024> buffer;
                boost::system::error_code error;
                size_t length = socket.read_some(boost::asio::buffer(buffer), error);
                std::string upLoadFile(buffer.data(), length);
                std::cout << "Client is downloading from server" << std::endl;
                std::ifstream file(upLoadFile, std::ios_base::binary | std::ios_base::ate);

                if (file) {
                    size_t file_size = file.tellg();
                    file.seekg(0);
                    char buffer[1024];
                    while (file.read(buffer, sizeof(buffer))) {
                        boost::asio::write(socket, boost::asio::buffer(buffer, sizeof(buffer)));
                    }
                    if (file.gcount() > 0) {
                        boost::asio::write(socket, boost::asio::buffer(buffer, file.gcount()));
                    }
                } else {
                    std::cerr << "Can't open the specified file or file doesn't exist: " << upLoadFile << std::endl;
                    
                }
            } catch (std::exception& e) {
                std::cout << "Error sending file: " << e.what() << std::endl;
                return 1;
            }
            
            std::cout << "download completed" << std::endl;
            sleep(1);
            boost::asio::write(socket, boost::asio::buffer("end", 3));
            setState("sleep");
            break;
        }
        case 3:
        {
            std::cout << "client has quit" << std::endl;
            break;
        }
        default:
        {
            std::cout << "Default" << std::endl;
        }
    }
    return 0;
}
void server::handle_accept(const boost::system::error_code& error, tcp::socket& socket) {
    if (!error) {
        std::cout << "Accepted a connection." << std::endl;
        currentState(socket);
    } else {
        std::cerr << "Error in async_accept: " << error.message() << std::endl;
    }
}

int server::execute()
{
  boost::asio::io_context io_context;
  tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), getPort()));
  std::cout << "Server started" << std::endl;
  for (;;)
    {
    tcp::socket socket(io_context);
    try {
        
        acceptor.accept(socket);
        std::cout << "Connection Accepted" << std::endl;
        currentState(socket);
        /*tcp::socket socket(io_context);
        acceptor.async_accept(socket,
            [&socket](const boost::system::error_code& error) {
                handle_accept(error, socket);
            }
        );*/
        
        
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
      
    }
    
    return 0;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
      std::cerr << "Usage: server <port>\n";
      return 1;
  }
  try
  {
    server myServer(atoi(argv[1]));
    myServer.execute();
  }
  catch (std::exception& e)
  {
    std::cout << "There was an error:" << std::endl;
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
