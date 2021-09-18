"use strict";

var ws = location.protocol === "https:" ? new WebSocket("wss://" + location.host + "/ws/", "tiktaktoe") : new WebSocket("ws://" + location.host + "/ws/", "tiktaktoe");

var gameState = {
    board : [],
    score : [],
    state : "",
    player : "",
    leader : "",
    winner : ""
};

function Board() {
    $(".game-board-cell").each(function(index) {
        switch(gameState.board[index]) {
            case -2 :
                $(this).addClass("cell--win");

            case -1 :
                $(this).addClass("cell-content__o");
                break;

            case 2 :
                $(this).addClass("cell--win");

            case 1 :
                $(this).addClass("cell-content__x");
                break;

            default :
                $(this).removeClass("cell-content__o cell-content__x cell--win");
        };
    });

    $("#player").attr("data", gameState.player);
    $("#score").attr("data", gameState.leader);
};

function Score() {
    $(".game-score-cell__number").each(function(index) {
        $(this).text(gameState.score[index]);
    });
};

function Reset() {
    $("#game").removeClass("state--tie state--win");
    $("#score").removeClass("player_1 player_2 ties");
};

function Tie() {
    // update with 1 sec delay
    setTimeout(function() {
        $("#game").addClass("state--tie");
        $("#score").addClass("ties");
        $("#game-tie").get(0).play();

        Score();
    }, 1000);
};

function Win() {
    // update with 1 sec delay
    setTimeout(function() {
        $("#game").addClass("state--win");
        $("#score").addClass(gameState.winner);
        $("#game-win").get(0).play();

        Score();
    }, 1000);
};

ws.onmessage = function(event) {
    var message = JSON.parse(event.data);

    // set once
    if (sessionStorage.length === 0) {
        sessionStorage.setItem("player", message.player);
    }

    // update every turn
    gameState.board = message.board;
    gameState.score = message.score;
    gameState.state = message.state;
    gameState.leader = message.leader;
    gameState.winner = message.winner;
    gameState.player = sessionStorage.getItem("player");

    switch(message.state) {
        case "setup" :
            
        case "reset" :
            Reset();

        case "update" :
            Board();
            Score();
            break;

        case "tie" :
            Board();
            Tie();
            break;

        case "win" :
            Board();
            Win();
            break;

        default :
            console.error("invalid action");
    };
};

ws.onopen = function() {
    $(".game-board-cell").click(function(event) {
        if (gameState.state === "tie" || gameState.state === "win") {
            // update with 1 sec delay
            setTimeout(function() {
                ws.send(JSON.stringify({
                    type : "reset"
                }));
            }, 1000);
        }
        else if (gameState.player === gameState.leader) {
            var cell = parseInt(event.target.dataset.id);

            if (gameState.board[cell] === 0) {
                if (gameState.player === "player_1") {
                    $("#note-low").get(0).play();
                }
                else {
                    $("#note-high").get(0).play();
                };

                ws.send(JSON.stringify({
                    type : "move",
                    cell : cell,
                    player : gameState.player
                }));
            };
        };
    });
};
