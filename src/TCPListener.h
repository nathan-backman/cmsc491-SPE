#ifndef TCPLISTENER_H_
#define TCPLISTENER_H_

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <netdb.h>
#include <string>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;

/**
 * The TCPListener class provides an interface to a TCP socket that listens for
 * and can correspond with incoming connections
 */
class TCPListener
{
    public:
        TCPListener()
        {
            connected = false;
            consockConnected = false;
        }

        /**
         * The class destructor - closes the socket
         */
        ~TCPListener()
        {
            Close();
        }


        /**
         * Binds the listener to the socket
         *
         * @param port The port to listen on
         *
         * @return \li \c -1 on failure to bind
         *         \li \c 0 on success
         */
        int Bind(unsigned short port)
        {
            listenSocket = socket(AF_INET, SOCK_STREAM, 0);


            if (listenSocket < 0)
            {
                cerr << "TCPListener.h::Bind() on socket() - " << strerror(errno)  << " (" << errno << ")" << endl;
                return -1;
            }

            connected = true;
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
            serverAddress.sin_port = htons(port);


            // lose the pesky "Address already in use" error message
            int yes=1;
            if (setsockopt(listenSocket,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1)
            {
                perror("setsockopt");
                return -1;
            }


            if (bind(listenSocket,(struct sockaddr *)&serverAddress, sizeof(struct sockaddr)) != 0)
            {
                cerr << "TCPListener.h::Bind() on bind()- " << strerror(errno)  << " (" << errno << ")" << endl;
                return -1;
            }

            listen(listenSocket, 8192);
            return 0;
        }


        int SetRecvTimeout(struct timeval tv) {
            return setsockopt(listenSocket,SOL_SOCKET,SO_RCVTIMEO, (char*)&tv, sizeof(tv));
        }


        /**
         * This is a blocking function that waits for an incoming connection
         *
         * @return \li \c -1 on failure to connect
         *         \li Descriptor of client socket on connection (always > 0)
         */
        int WaitForConnection()
        {
            if (consockConnected)
            {
                close(connSocket);
                consockConnected = false;
            }
            socklen_t clientAddressLength = sizeof(clientAddress);
            connSocket = accept(listenSocket,
                    (struct sockaddr *) &clientAddress,
                    &clientAddressLength);
            if (connSocket < 0)
            {
                //cerr << "cannot accept connection ";
                return -1;
            }
            consockConnected = true;
            return connSocket;
        }


        /**
         * Calls a blocking function that returns on an established connection or
         * after a timeout period has elapsed
         *
         * @param timeout The period after which the function stops listening for a
         * connection (in milliseconds)
         * 
         * @return \li \c < \c 0 on error 
         *         \li \c 0 on timeout
         *         \li Descriptor of client socket on connection (always > 0)
         */
        int Poll(unsigned int timeout)
        {
            int retCode;
            struct pollfd pfd;
            pfd.fd = listenSocket;
            pfd.events = POLLIN;

            retCode = poll( &pfd, 1, timeout);

            // timeout or error on poll
            if (retCode <= 0)
                return retCode;

            // Return descriptor of client connection
            if (pfd.revents == POLLIN)
                return WaitForConnection();

            //Timeout
            return 0;
        }


        /**
         * Gets a message from the connected computer
         *
         * @param msgbuf A pointer to the buffer on which to store the message
         * @param msgbufsize The size of the message to be received
         *
         * @return The number of bytes read
         */
        int GetData(char *msgbuf, int msgbufsize)
        {
            int bytesRead = 0;
            bool firstRead = true;
            while (bytesRead < msgbufsize)
            {
                int retCode = recv(connSocket, (msgbuf + bytesRead), (msgbufsize - bytesRead), 0);
                if (firstRead)
                {
                    firstRead = false;
                    if (retCode == 0)
                        return 0;
                }
                if (retCode == -1)
                {
                    //cerr << "TCPListener.h::GetData() - " << strerror(errno)  << " (" << errno << ")" << endl;
                    return -1;
                }
                else
                    bytesRead  += retCode;
            }
            return bytesRead;
        }


        /**
         * Gets a message from the specified file descriptor
         *
         * @param fd The file descriptor that we will read from
         * @param msgbuf A pointer to the buffer on which to store the message
         * @param msgbufsize The size of the message to be received
         *
         * @return The number of bytes read
         */
        int GetData(int fd, char *msgbuf, int msgbufsize)
        {
            int bytesRead = 0;
            bool firstRead = true;
            while (bytesRead < msgbufsize)
            {
                int retCode = recv(fd, (msgbuf + bytesRead), (msgbufsize - bytesRead), 0);
                if (firstRead)
                {
                    firstRead = false;
                    if (retCode == 0)
                        return 0;
                }
                if (retCode == -1)
                {
                    cerr << "TCPListener.h::GetData(fd) - " << strerror(errno)  << " (" << errno << ")" << endl;
                    exit(1);
                    return -1;
                }
                else
                    bytesRead  += retCode;
            }
            return bytesRead;
        }

        /**
         * Gets a string from the previously established socket connection. The
         * string read from the stream is to be terminated at a '\n' character.
         *
         * @return A string containing the bytes read. It does not include the
         *         newline character.
         */
        string GetLine()
        {
            char ch;
            string retStr = "";
            int retVal;
            while ((retVal = recv(connSocket, &ch, 1, 0)) && ch != '\n') {
                retStr = retStr + ch;
                if (retVal == -1) {
                    cerr << "TCPListener.h::GetLine() - " << strerror(errno)  << " (" << errno << ")" << endl;
                    exit(1);
                }
            }
            return retStr;
        }


        /**
         * Sends a message to the connected computer
         *
         * @param msgbuf A pointer to the buffer containing the message
         * @param msgbufsize The size of the message contained in the buffer
         *
         * @return The number of bytes sent
         */
        int SendData(const char *msgbuf, int msgbufsize)
        {
            int bytesWritten = 0;
            while (bytesWritten < msgbufsize)
            {
                int retCode = send(connSocket, (msgbuf + bytesWritten), (msgbufsize - bytesWritten), 0);
                if (retCode == -1)
                {
                    cerr << "TCPListener.h::SendData() - " << strerror(errno)  << " (" << errno << ")" << endl;
                    return -1;
                }
                else
                    bytesWritten += retCode;
            }
            return bytesWritten;
        }


        int SendData(int fd, char *msgbuf, int msgbufsize)
        {
            int bytesWritten = 0;
            while (bytesWritten < msgbufsize)
            {
                int retCode = send(fd, (msgbuf + bytesWritten), (msgbufsize - bytesWritten), 0);
                if (retCode == -1)
                {
                    cerr << "TCPListener.h::SendData(fd) - " << strerror(errno)  << " (" << errno << ")" << endl;
                    return -1;
                }
                else
                    bytesWritten += retCode;
            }
            return bytesWritten;
        }


        /**
         * Closes the open connection
         *
         * @return \li \c -1 on failure to close connection
         *         \li \c 0 on connection closed
         */
        int Close()
        {
            if ( connected == true )
            {
                if ( close(listenSocket) != 0 )
                {
                    cerr << "TCPListener.h::Close() - " << strerror(errno)  << " (" << errno << ")" << endl;
                    return -1;
                }
                connected = false;
            }

            if ( consockConnected == true )
            {
                if ( close(connSocket) != 0 )
                {
                    cerr << "TCPListener.h::Close() - " << strerror(errno)  << " (" << errno << ")" << endl;
                    return -1;
                }
                consockConnected = false;
            }

            return 0;
        }


        int listenSocket;   ///< The descriptor of the listening socket
    private:
        bool connected;
        bool consockConnected;
        int connSocket;  ///< The descriptor of the connecting socket
        struct sockaddr_in serverAddress; ///< The local address of the listener
        struct sockaddr_in clientAddress; ///< The address of the client
};

#endif /* TCPLISTENER_H_ */
