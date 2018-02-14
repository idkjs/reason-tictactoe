[%bs.raw {|require('./tictactoe.css')|}];

open Player;

open Board;

type progress =
  | Turn(player)
  | Win(player)
  | Draw;

type state = {
  board,
  progress
};

type action =
  | TokenAdded(rowId, colId);

let component = ReasonReact.reducerComponent("TicTacToe");

let make = (_children) => {
  ...component,
  initialState: () => {
    board: ((Empty, Empty, Empty), (Empty, Empty, Empty), (Empty, Empty, Empty)),
    progress: Turn(X)
  },
  reducer: (action, state) =>
    switch (action, state.progress) {
    | (TokenAdded(rId, cId), Turn(p)) =>
      let currentToken = getTokenForPlayer(p);
      let updateBoard = updateBoard(state.board, rId, cId, currentToken);
      let rowComplete = getRowLine(updateBoard, rId) |> isLineFullWith(currentToken);
      let colComplete = getColumnLine(updateBoard, cId) |> isLineFullWith(currentToken);
      if (rowComplete || colComplete) {
        ReasonReact.Update({progress: Win(p), board: updateBoard})
      } else if (! isBoardFull(updateBoard)) {
        ReasonReact.Update({
          progress: updateBoard === state.board ? Turn(p) : Turn(p === X ? O : X),
          board: updateBoard
        })
      } else {
        ReasonReact.Update({progress: Draw, board: updateBoard})
      }
    | _ => ReasonReact.NoUpdate
    },
  render: ({state, send}) => {
    let title =
      switch state.progress {
      | Turn(p) => "Player" ++ p_to_str(p) ++ "'s turn"
      | Win(p) => "Player" ++ p_to_str(p) ++ "won"
      | Draw => "It's a draw"
      };
    <div className="tic-tac-toe">
      <div className="title"> (ReasonReact.stringToElement("Tic Tac Toe - " ++ title)) </div>
      <div className="board">
        (
          [R1, R2, R3]
          |> List.map((rId) => [C1, C2, C3] |> List.map((cId) => (rId, cId)))
          |> List.flatten
          |> List.map(
               ((rId, cId)) =>
                 <div className="cell" onClick=((_event) => send(TokenAdded(rId, cId)))>
                   (ReasonReact.stringToElement(token_to_str(getToken(state.board, rId, cId))))
                 </div>
             )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
    </div>
  }
};