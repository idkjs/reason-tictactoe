type lottieAnimation = {setSpeed: int => unit};

type action =
  | Pause;

type optionProps = {
  loop: bool,
  autoplay: bool,
  animationData: string,
};

type animationOptions = {
  container: Dom.element,
  renderer: string,
  loop: bool,
  autoplay: bool,
  animationData: string,
};

[@bs.val] [@bs.module "lottie-web"]
external loadAnimation: animationOptions => lottieAnimation = "loadAnimation";

let defaultSize = "100%";

type state = {
  ref: ref(option(Dom.element)),
  animation: ref(option(lottieAnimation)),
};

[@react.component]
let make =
    /* ~eventListeners=[], */
    (
      ~options: optionProps,
      ~width=defaultSize,
      ~height=defaultSize,
      /* ~isStopped=false, */
      /* ~isPaused=false, */
      /* ~speed=1, */
      /* ~direction=1, */
      ~ariaRole="button",
      ~isClickToPauseDisabled=false,
      ~title="",
    ) => {
  // initialState: () => {ref: ref(None), animation: ref(None)},
  let theRef: React.ref(Js.Nullable.t(Dom.element)) =
    React.useRef(Js.Nullable.null);
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Pause => state
        },
      {ref: ref(None), animation: ref(None)},
    );
  React.useEffect(() => {
    switch (state.ref^) {
    | Some(ref) =>
      Js.log(ref);
      let animationOptions = {
        container: ref,
        renderer: "svg",
        loop: options.loop !== false,
        autoplay: options.autoplay !== false,
        animationData: options.animationData,
      };
      let animation = loadAnimation(animationOptions);
      state.animation := Some(animation);
    | None => ()
    };
    None;
  });

  let clickPause = _event => isClickToPauseDisabled ? () : dispatch(Pause);
  let lottieStyles =
    ReactDOMRe.Style.make(
      ~overflow="hidden",
      ~marginTop="0",
      ~marginRight="auto",
      ~marginBottom="0",
      ~marginLeft="auto",
      ~height,
      ~width,
      (),
    );
  <div
    ref={ReactDOM.Ref.domRef(theRef)}
    style=lottieStyles
    title
    role=ariaRole
    onClick=clickPause
  />;
};
