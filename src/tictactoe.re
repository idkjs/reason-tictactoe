[%raw {|"require('./tictactoe.css')"|}];

let animationDataPlayer1 = [%raw {|"require('./x.json')"|}];

let animationDataPlayer2 = [%raw {|"require('./o.json')"|}];

open Player;

open Board;

type progress =
  | Turn(player)
  | Win(player)
  | Draw;

type state = {
  board,
  progress,
};

type action =
  | TokenAdded(rowId, colId);

let isWinByToken = (board: board, rId: rowId, cId: colId, token: token) =>
  if (getRowLine(board, rId) |> isLineFullWith(token)) {
    true;
  } else if (getColumnLine(board, cId) |> isLineFullWith(token)) {
    true;
  } else {
    getDiagonalLines(board)
    |> List.map(isLineFullWith(token))
    |> List.fold_left((res, isFull) => res || isFull, false);
  };
[@react.component]
let make = () => {
  let initialState = {
    board: (
      (Empty, Empty, Empty),
      (Empty, Empty, Empty),
      (Empty, Empty, Empty),
    ),
    progress: Turn(Cross),
  };
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action, state.progress) {
        | (TokenAdded(rId, cId), Turn(p)) =>
          let currentToken = Mark(p);
          let updateBoard = updateBoard(state.board, rId, cId, currentToken);
          if (isWinByToken(updateBoard, rId, cId, currentToken)) {
            {progress: Win(p), board: updateBoard};
          } else if (!isBoardFull(updateBoard)) {
            {
              progress:
                updateBoard === state.board
                  ? Turn(p) : Turn(p === Cross ? Circle : Cross),
              board: updateBoard,
            };
          } else {
            {progress: Draw, board: updateBoard};
          };
        | _ => state
        },
      initialState,
    );

  let title =
    switch (state.progress) {
    | Turn(p) => "Player " ++ toString(p) ++ " turn"
    | Win(p) => "Player " ++ toString(p) ++ " won"
    | Draw => "It's a draw"
    };
  <div className="tic-tac-toe">
    <div className="title">
      <div className="title__main"> {React.string("TIC TAC TOE")} </div>
      <div className="title__sub"> {React.string(title)} </div>
    </div>
    <div className="board">
      {[R1, R2, R3]
       |> List.map(rId => [C1, C2, C3] |> List.map(cId => (rId, cId)))
       |> List.flatten
       |> List.map(((rId, cId)) =>
            <div
              className="board__cell"
              key={row_to_str(rId) ++ col_to_str(cId)}
              onClick={_event => dispatch(TokenAdded(rId, cId))}>
              {switch (getToken(state.board, rId, cId)) {
               | Mark(Cross) =>
                 <Lottie
                   options={
                     animationData: animationDataPlayer1,
                     loop: true,
                     autoplay: true,
                   }
                 />
               | Mark(Circle) =>
                 <Lottie
                   options={
                     animationData: animationDataPlayer2,
                     loop: false,
                     autoplay: true,
                   }
                 />
               | Empty => React.string("")
               }}
            </div>
          )
       |> Array.of_list
       |> React.array}
    </div>
  </div>;
};
