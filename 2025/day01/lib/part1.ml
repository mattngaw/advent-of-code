let solve (filename : string) : int =
  let read (filename : string) : string list =
    In_channel.with_open_text filename In_channel.input_lines
  in
  let preprocess : string list -> int list =
    let transform (s : string) : int =
      let len = String.length s in
      let direction = String.get s 0 in
      let magnitude = String.sub s 1 (len - 1) |> Stdlib.int_of_string in
      if direction = 'L' then -magnitude else magnitude
    in
    List.map transform
  in
  let modulo x y =
    let result = x mod y in
    if result >= 0 then result else result + y
  in
  let rec step (zeros : int) (dial : int) : int list -> int = function
    | [] ->
        zeros
    | x :: xs ->
        let dial' = modulo (dial + x) 100 in
        let zeros' = if dial' = 0 then zeros + 1 else zeros in
        step zeros' dial' xs
  in
  let base : int list -> int = step 0 50 in
  filename |> read |> preprocess |> base
