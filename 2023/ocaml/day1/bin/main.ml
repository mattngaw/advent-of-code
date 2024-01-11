let file : string = "input.txt"

let explode (s : string) : char list = 
    List.init (String.length s) (String.get s)

let is_digit (c : char) : bool =
    '0' <= c && c <= '9'

let digit_to_int (c : char) : int =
    Char.code c - Char.code '0'

let rec first_last (xs : 'a list) : 'a * 'a = 
    match xs with
    | [] -> failwith "Not enough elements"
    | [x] -> (x,x)
    | [x1;x2] -> (x1,x2) 
    | x::_::xs -> first_last (x::xs)

let part1 (lines : string list) : int =
    lines
    |> List.map (fun line ->
        explode line
        |> List.filter is_digit
        |> List.map digit_to_int
        |> first_last
        |> (fun (x1, x2) -> x1 * 10 + x2)
    )
    |> List.fold_left ( + ) 0

let string_to_digits =
    [ ("one", 1)
    ; ("two", 2)
    ; ("three", 3)
    ; ("four", 4)
    ; ("five", 5)
    ; ("six", 6)
    ; ("seven", 7)
    ; ("eight", 8)
    ; ("nine", 9)
    ]

let rec find_digits (s : string) : int list =
    if s = String.empty then [] else
    let next_len = String.length s - 1 in
    let substring = String.sub s 1 next_len in
    let next_digits = find_digits substring in
    match
        List.find_opt (fun (word, _) ->
            String.starts_with ~prefix:word s
        ) string_to_digits
    with
    | None -> 
        let c = String.get s 0 in
        let digit = digit_to_int c in
        if is_digit c then digit :: next_digits
        else next_digits
    | Some((_, digit)) ->
        digit :: next_digits
    
let part2 (lines : string list) : int =
    lines
    |> List.map (fun line ->
        find_digits line
        |> first_last
        |> (fun (x1, x2) -> x1 * 10 + x2)
    )
    |> List.fold_left ( + ) 0

let () =
    let ic = open_in file in
    let lines = In_channel.input_lines ic in
    let () = print_int (part1 lines) in
    let () = print_newline () in
    let () = print_int (part2 lines) in
    close_in ic
