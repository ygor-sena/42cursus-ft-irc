#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class TCPServer
{
    private:
        int server_socket;
        int port;
        std::string password;
        static const int MAX_PENDING_CONNECTIONS = 10;
        static const int BUFFER_SIZE = 1024;

    public:
        TCPServer(int port, const std::string& password) : server_socket(-1), port(port), password(password) {}

        bool start()
        {
            // Criar o socket TCP
            server_socket = socket(AF_INET, SOCK_STREAM, 0);
            if (server_socket == -1)
            {
                std::cerr << "Erro ao criar o socket: " << strerror(errno) << std::endl;
                return false;
            }

            // Configurar o endereço e a porta do servidor
            sockaddr_in server_addr;
            std::memset(&server_addr, 0, sizeof(server_addr));
            server_addr.sin_family = AF_INET;
            server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
            server_addr.sin_port = htons(port);

            // Vincular o socket ao endereço e à porta
            if (bind(server_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1)
            {
                std::cerr << "Erro ao vincular o socket: " << strerror(errno) << std::endl;
                close(server_socket);
                return false;
            }

            // Colocar o socket em modo de escuta
            if (listen(server_socket, MAX_PENDING_CONNECTIONS) == -1)
            {
                std::cerr << "Erro ao colocar o socket em modo de escuta: " << strerror(errno) << std::endl;
                close(server_socket);
                return false;
            }

            std::cout << "Servidor TCP escutando na porta " << port << std::endl;

            return true;
        }

        // Manipular um cliente em uma nova thread
        static void* client_handler(void* arg)
        {
            int client_socket = *static_cast<int*>(arg);
            delete static_cast<int*>(arg); // Liberar memória alocada para o descritor do socket do cliente
            char buffer[BUFFER_SIZE];

            // Solicitar senha ao cliente
            const char* prompt = "Digite a senha: ";
            send(client_socket, prompt, strlen(prompt), 0);

            // Receber a senha do cliente
            ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
            if (bytes_received == -1)
            {
                std::cerr << "Erro ao receber dados do cliente: " << strerror(errno) << std::endl;
                close(client_socket);
                pthread_exit(NULL);
            }
            else if (bytes_received == 0)
            {
                std::cout << "Cliente desconectado" << std::endl;
                close(client_socket);
                pthread_exit(NULL);
            }
            buffer[bytes_received] = '\0';


            //TODO: Implementar a verificação da senha, por enquanto, a senha é sempre correta
            // // Verificar se a senha está correta
            // if (strcmp(buffer, password.c_str()) != 0) {
            //     const char* error_msg = "Senha incorreta. Conexão encerrada.\n";
            //     send(client_socket, error_msg, strlen(error_msg), 0);
            //     close(client_socket);
            //     pthread_exit(NULL);
            // }

            // Senha correta, continuar interagindo com o cliente
            const char* success_msg = "Senha correta. Conexão estabelecida.\n";
            send(client_socket, success_msg, strlen(success_msg), 0);

            while (true)
            {
                // Receber dados do cliente
                ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
                if (bytes_received == -1)
                {
                    std::cerr << "Erro ao receber dados do cliente: " << strerror(errno) << std::endl;
                    break;
                }
                else if (bytes_received == 0)
                {
                    std::cout << "Cliente desconectado" << std::endl;
                    break;
                }
                else
                {
                    buffer[bytes_received] = '\0';
                    std::cout << "Mensagem recebida do cliente: " << buffer << std::endl;
                }
            }

            // Fechar o socket do cliente
            close(client_socket);

            // Encerrar a thread
            pthread_exit(NULL);
        }

        // Aceitar conexões de clientes e criar threads para manipulá-los
        void run()
        {
            while (true)
            {
                // Aceitar a conexão de um cliente
                sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int* client_socket = new int(accept(server_socket, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len));
                if (*client_socket == -1)
                {
                    std::cerr << "Erro ao aceitar a conexão: " << strerror(errno) << std::endl;
                    delete client_socket;
                    continue;
                }

                // Criar uma nova thread para manipular o cliente
                pthread_t tid;
                if (pthread_create(&tid, NULL, &client_handler, static_cast<void*>(client_socket)) != 0)
                {
                    std::cerr << "Erro ao criar thread para manipular o cliente" << std::endl;
                    close(*client_socket);
                    delete client_socket;
                }
            }
        }

        // Autenticar um usuário
        bool authenticate(const std::string& input_password)
        {
            return password == input_password;
        }

        // Fechar o socket do servidor
        void stop()
        {
            close(server_socket);
        }
};

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Uso: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    int port = std::atoi(argv[1]);
    std::string password = argv[2];

    TCPServer server(port, password);
    if (!server.start())
    {
        std::cerr << "Falha ao iniciar o servidor" << std::endl;
        return 1;
    }

    server.run();

    server.stop();

    return 0;
}