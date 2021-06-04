const PORT = process.env.PORT || 4000

const express   = require('express')
const WebSocket = require('ws')

let gameState = {
  players: ['red', 'blue'],
  whosNext: 0,
  numPlayers: 0,
  board: [
    0, 0, 0,
    0, 0, 0,
    0, 0, 0
  ]
}

// Handles changing the state of the board
//   also make sure only the right player is allowed to move
var playersMove = (player, cellID) => {
  if (player === gameState.players[gameState.whosNext]) {
    let cellValue = 0;
    if (player === 'red') {
      cellValue = 1;
    } else if (player === 'blue') {
      cellValue = -1;
    }
    gameState.board[cellID] = cellValue
    if (gameState.whosNext >= 1) {
      gameState.whosNext = 0;
    } else {
      gameState.whosNext += 1;
    }
  }
}

var resetBoard = () => {
  gameState.whosNext = 0;
  gameState.board = [0, 0, 0, 0, 0, 0, 0, 0, 0];
}

// Setup Express and start listening on PORT
const app = express()
  .use(express.static('public'))
  .listen(
    PORT,
    () => console.log(`Listening on ${PORT}`)
  );

// WebSocket Creation
const wss = new WebSocket.Server({server: app});

var updateClientState = () => {
  wss.clients.forEach( (client) => {
    if (client.readyState === WebSocket.OPEN) {
      var message = {
        type:     'update',
        whosTurn: gameState.players[gameState.whosNext],
        board:    gameState.board
      }
      client.send(JSON.stringify(message));
    }
  })
}

// This gets called when a new player signs in.
wss.on('connection', (ws) => {

  // Send the game setup to the client
  var message = {
    type: 'setup',
    playerData: {
      whosTurn: gameState.players[gameState.whosNext],
      playerID: gameState.players[gameState.numPlayers++],
      board:    gameState.board
    }
  }
  ws.send(JSON.stringify(message))


  // when we receive a message from the client
  ws.on('message', (message) => {
    let action = JSON.parse(message);

    switch(action.type) {
      case 'move':
        playersMove(action.playerID, action.cellID);
        updateClientState();
        break;
      default:
        console.error('Invalid Message');
    }
  })

  ws.on('close', () => console.log("Client left."))
})
