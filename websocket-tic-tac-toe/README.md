# Websockets

## Agenda

1. What are Websockets
2. Uses
3. Look at the Spec (on MDN)
4. Websockets on the Server
4. Demo


## What are Websockets

Websocket are a fully-duplex message protocol that allows both the
client/server to have a two connection that remains open.

It allows application developers to *realtime* communicate with the
server and to receive updates from the server. This is different from
**HTTP** since HTTP  request only go from client -> server.

## Websocket Users

We sockets can and should be used in applications that require
*realtime* communication. It uses include:

* Chat apps
* Collaborative Text Editors
* Notifications
* Financial applications (example: stock tickers)
* Multiplayer

Apps that only deal with submitting resources and then retrieving them
(the majority of apps) will have better performance without websockets
so choose carefully whether or not to use them.

## Websocket Spec

The official spec is located
on [MDN](https://developer.mozilla.org/en-US/docs/Web/API/WebSocket).

The Websocket API is really small a simple. For the most part you set
up an event handler on *message* and you use either the *send()*
command or the *close()* command.

Servers on the other hand don't have builtin WebSocket support. A
great library to add support for *node*
is [WS](https://github.com/websockets/ws)!

## DEMO

For the demo we created a tic tac toe game that allows two players to
play against each other. You can find the code at (https://github.com/rob0t7/websocket-tic-tac-toe)
