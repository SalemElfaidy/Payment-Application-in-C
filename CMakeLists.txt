cmake_minimum_required(VERSION 3.21)
project(Payment_Application__in_C___FWD C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(Application)
include_directories(Card)
include_directories(Server)
include_directories(Terminal)
include_directories(Data Types)

add_executable(Payment_Application__in_C___FWD
        Application/app.c
        Application/app.h
        Server/server.h
        Terminal/terminal.h
        Card/card.h
        Application/app.c
        Server/server.c
        Terminal/terminal.c
        Card/card.c
        "Data Types/dataTypes.h"
        )

add_executable(Card Card/card.c
        Card/card.h
        "Data Types/dataTypes.h")

add_executable(Terminal Terminal/terminal.c
        Terminal/terminal.h
        Card/card.h
        Card/card.c)

add_executable(Server Server/server.c
        Server/server.h
        Terminal/terminal.h)
