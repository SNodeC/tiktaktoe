// var ws = new WebSocket('wss://titan.home.vchrist.at:8088/ws/', 'tiktaktoe');

if (location.protocol == "https:") {
  var ws = new WebSocket('wss://' + location.host + '/ws/', 'tiktaktoe');
} else {
  var ws = new WebSocket('ws://' + location.host + '/ws/', 'tiktaktoe');
}

var gameState = {
  whosTurn: undefined,
  playerID: undefined,
  board: [0, 0, 0, 0, 0, 0, 0, 0, 0]
};


// This fills in the DOM with the initial game state
var initializeBoard = () => {
  document.querySelector('.game-info__players-name').innerText = `${gameState.playerID} Player`;
  updateBoard();
}

// This function updates the board and whos turn it is.
var updateBoard = () => {
  let myTurn = gameState.whosTurn === gameState.playerID ? "My Turn" : "Opponents Turn"
  document.querySelector('.game-info__players-turn').innerText = myTurn;
  let board = gameState.board;
      let cellValue;
    if (gameState.playerID==0) {
        cellValue=-1;
    } else{
        cellValue=1;
    }
  for(let i = 0; i < board.length; i++) {
    let element = document.querySelector(`.game-board__cell[data-id='${i}']`)
    element.classList.remove('game-board__cell--blue')
    element.classList.remove('game-board__cell--red')
    if (board[i] < 0) {
      element.classList.add('game-board__cell--blue')
    }
    if (board[i] > 0) {
      element.classList.add('game-board__cell--red')
    }
    
  }
  playerWon();
}

var playerWon = () => {
    let gameOver = gameState.whosTurn === gameState.playerID ? "Lost 😱" : "Won 🎉"
    let board = gameState.board;
    
    if (board[0]==1 && board[1]==1 && board[2]==1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[3]==1 && board[4]==1 && board[5]==1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[6]==1 && board[7]==1 && board[8]==1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[0]==1 && board[3]==1 && board[6]==1){
        document.querySelector('.game-info__state').innerText = gameOver        
    }else if (board[1]==1 && board[4]==1 && board[7]==1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[2]==1 && board[5]==1 && board[8]==1){
        document.querySelector('.game-info__state').innerText = gameOver;
    } else if (board[0]==1 && board[4]==1 && board[8]==1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[2]==1 && board[4]==1 && board[6]==1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }
    
        if (board[0]==-1 && board[1]==-1 && board[2]==-1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[3]==-1 && board[4]==-1 && board[5]==-1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[6]==-1 && board[7]==-1 && board[8]==-1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[0]==-1 && board[3]==-1 && board[6]==-1){
        document.querySelector('.game-info__state').innerText = gameOver        
    }else if (board[1]==-1 && board[4]==-1 && board[7]==-1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[2]==-1 && board[5]==-1 && board[8]==-1){
        document.querySelector('.game-info__state').innerText = gameOver;
    } else if (board[0]==-1 && board[4]==-1 && board[8]==-1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }else if (board[2]==-1 && board[4]==-1 && board[6]==-1){
        document.querySelector('.game-info__state').innerText = gameOver;
    }
    
    else if (board[0]!==0 && board[1]!==0  && board[2]!==0 && board[3]!==0 && board[4]!==0 && board[5]!==0 && board[6]!==0 && board[7]!==0  && board[8]!==0) {
      document.querySelector('.game-info__state').innerText = "Draw"; 
    }
}

// This responds to the server push messages
ws.addEventListener('message', (message) => {
  let action = JSON.parse(message.data);
  let loadingEl = document.querySelector('.game-loading');
  switch(action.type) {
    case 'setup':
      gameState = action.playerData;
      initializeBoard();
      loadingEl.style.display = 'none';
      break
    case 'update':
      loadingEl.style.display = 'block';
      gameState.whosTurn = action.whosTurn;
      gameState.board    = action.board;
      updateBoard();
      loadingEl.style.display = 'none';
      break;        
    default:
      console.error("Invalid action");
  }
});


ws.addEventListener('open', () => {
  document
    .querySelector('.game-board')
    .addEventListener('click', (event) => {
      let element = event.target;
      let message = {
        type:     'move',
        playerID: gameState.playerID,
        cellID:   parseInt(element.dataset.id, 10)
      }
      ws.send(JSON.stringify(message));
    })
})
