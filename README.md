<p align="center">
    <img src="https://github.com/ygor-sena/42cursus-ft-irc/assets/102881479/51bb7248-3bc7-4fb0-9e41-c71bedb7b3d4">
</p>

<p align="center">
    <img src="https://img.shields.io/badge/OS-Linux-blue" alt="OS">
    <img src="https://img.shields.io/badge/Language-C%2B%2B98-blue.svg" alt="Language">
    <img src="https://img.shields.io/badge/Grade-125%2F125-brightgreen.svg" alt="Grade">
    <img src="https://img.shields.io/badge/Status-Completed-brightgreen.svg" alt="Status">
</p>


📣 Introduction
---------------
The 12th project of 42's curriculum is about creating our own IRC server. An actual IRC client will be used to connect to the server and test it. Internet is ruled by solid standards protocols that allow connected computers to interact with each other. It’s always a good thing to know.

📜 Requirements
---------------

We have implemented both the mandatory and bonus requirements of this project. The following table summarizes all the possible commands that our IRC server is capable of processing.

|Command|Usage|Flags|Description|
|:--|:--|:--|:--|
|`PASS`| `PASS <password> `|- |Connect to the IRC server by providing a password.|
|`USER`| `USER <username> `| - | Create a new username. |
|`NICK`|`NICK <nickname> ` | - | Create a new nickname.|
|`JOIN`|`JOIN <channel> [<key>]` | - | Join a channel, if it already exists, otherwise create a new channel. It's also possible to provide a key (password) to access the channel.|
|`INVITE`|`INVITE <nickname> <channel>` | - | Invite a client to a channel.|
|`TOPIC`| `TOPIC <channel> [<topic>]`| - | Change or view the channel topic.|
|`PRIVMSG`| `PRIVMSG <receiver>{,<receiver>} <text to be sent>`| - | Sends a message to one or more clients or channels. If there are more than one receiver, they must be separated by `,`.| 
|`MODE`| `MODE <channel> <flag> [<limit>] [<user>]`| `+i` or `-i` | Change the channel's mode. This flag sets/removes Invite-only channel.|
||| `+t` or `-t` | Change the channel's mode. This flag sets/removes the restrictions of the `TOPIC` command to channel operators. |
||| `+k` or `-k` | Change the channel's mode. This flag sets/removes the channel key. |
||| `+o` or `-o` | Change the channel's mode. This flag gives/takes channel operator privilege. |
||| `+l` or `-l` | Change the channel's mode. This flag sets/removes the user limit to channel. |
|`PART`| `PART <channel>`| - | Leave a channel.|
|`KICK`| `KICK <channel> <user> [<comment>]`| - | Eject a client from the channel. It's also possible to provide a commentary.|
|`QUIT`| `QUIT`| - | Closes the IRC server connection.|
|`MARVINBOT`| `MARVINBOT <flag>` |`marvin`| Marvin presents himself!|
||| `quote` | Marvin recites a philosophy quote!|
||| `time` | Marvin answers the current time.|
||| `whoami` | Marvin answers who you are.|
|| |`whois` | Marvin gets information about other clients logged on the IRC server.|

>[!IMPORTANT]
> Some command have to meet some requirements for its usage by the client:
> - To execute the command `KICK` and `MODE`, the client must have channel operator privileges.
> - To execute the command `MARVINBOT`, the client must have joined at least one channel.
> - If the command requires a channel, the channel name must begin with `#`.

⚒️ How to compile and run the project
-------------------------------------

#### 1) Copy this repository to your local workstation

```
git clone https://github.com/ygor-sena/42cursus-miniRT.git
```

#### 2) Install the required libraries to run the project and the unit tests

```
sudo apt-get install libcriterion-dev
```

>[!NOTE]
>If your operating system is different than Ubuntu `version >= 21.04`, then check the official documentation about how to proceed with Criterion framework installation [here](https://github.com/Snaipe/Criterion.git).

#### 3) Compile the project with Makefile

```
make
```

To compile and execute the tests, just run `make` inside the folder `42-IRC/tests`.

#### 4) Launch the IRC server

To launch the IRC server, you must provide a port number and a password. For example, the command below launches the IRC server to listen to port number `4444` and the password to access it is `12345`.

```
./ircserv 4444 12345
```

#### 5) Connect one or more clients to the IRC server and have fun!

There are two ways to connect a client to the IRC server: via CLI or IRC client of your choice. 

> [!NOTE]
> We used [Hexchat](https://hexchat.github.io/) as **our** IRC client of reference. Therefore, we recommend that you use it as well if you want to connect to the IRC server using a GUI. Check its official documentation about how to install Hexchat IRC client on you local machine [here](https://hexchat.github.io/downloads.html).

To connect to the IRC server on port number 4444 using the command line, open a new terminal and run the following command:

```
nc localhost 4444
```

To use the server, you will have to authenticate the client connection by providing a password and registering the client on the server by creating an username and a nickname. For example:

```
PASS 12345
USER username
NICK nickname
```

After that, the client is allowed to use all the other IRC command implemented for this project as specified [here](https://github.com/ygor-sena/42cursus-ft-irc?tab=readme-ov-file#-requirements).

To connect to the IRC server using Hexchat, you have to create a new network called `localhost/4444` and select the following options:
- [X] Connect to selected server only
- [X] Connect to this network automatically
- [X] Accept invalid SSL certificates
- Login method: Server password (/PASS password)
- Last but not least, fill the fields `Nick name`, `Second choice`, `Real name`, `User name` and `Password`

After the setting, you can click on `Connect` to connect with the IRC server and use the implemented commands on the prompt command line. Since the file management is done by IRC clients, it's possible to send a file from a client to another one if both clients are using Hexchat.

📖 References
--------------

Main references:
- [Internet Relay Chat](https://chi.cs.uchicago.edu/chirc/irc.html) by The UChicago χ-Projects.
- [RFC 1459 - Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459#section-4.2.7) by J. Oikarinen and D. Reed.
- [Small IRC Server \[ft_irc 42 network\]](https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9) by Ahmed Fatir.

About network programming:
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/html/split-wide/index.html) by Brian “Beej Jorgensen” Hall.
- [Sockets and Network Programming in C](https://www.codequoi.com/en/sockets-and-network-programming-in-c/) by Mia Combeau.
- [Sockets in Computer Network](https://www.geeksforgeeks.org/socket-in-computer-network/) by [GeeksforGeeks](https://www.geeksforgeeks.org/).

About recommended design patterns for this project:
- Many-to-many relationships: [Mediator Design Pattern in C++](https://www.vishalchovatiya.com/mediator-design-pattern-in-modern-cpp/) by [Vishal Chovativa](https://www.vishalchovatiya.com/)
- One-to-many relationships: [Observer Design Pattern in C++](https://www.vishalchovatiya.com/observer-design-pattern-in-modern-cpp/) by [Vishal Chovativa](https://www.vishalchovatiya.com/)
