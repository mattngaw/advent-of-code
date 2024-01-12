let file : string = "input.txt"

let parse_round (round_str : string) : int * int * int =
    let color_strs = String.split_on_char ',' (String.trim round_str) in
    let color_strs = color_strs |> List.map (String.trim) in
    color_strs |> List.map (fun color_str ->
        let split = String.split_on_char ' ' color_str in
        let color = List.nth split 1 in
        let count_str = List.nth split 0 in
        let count = Stdlib.int_of_string count_str in
        (color, count)
    ) |> List.fold_left (fun (r, g, b) (color, count) ->
        match color with
        | "red" -> (count, g, b)
        | "green" -> (r, count, b)
        | "blue" -> (r, g, count)
        | _ -> failwith "Unexpected color!"
    ) (0, 0, 0)
    
let parse (line : string) : int * (int * int * int) list =
    let split : string list = String.split_on_char ':' line in

    let game_str : string = List.nth split 0 in
    let id_str : string = String.sub game_str 5 (String.length game_str - 5) in
    let id : int = Stdlib.int_of_string id_str in

    let rounds_str : string = List.nth split 1 in
    let round_strs : string list = String.split_on_char ';' rounds_str in

    round_strs 
    |> List.map (parse_round)
    |> (fun rounds -> (id, rounds))

let part1_filter (_, rounds) : bool =
    rounds |> List.map (fun (r, g, b) -> r <= 12 && g <= 13 && b <= 14)
    |> List.fold_left ( && ) true

let part1 (lines : string list) : int =
    List.map (parse) lines
    |> List.filter (part1_filter)
    |> List.map (fun (id, _) -> id)
    |> List.fold_left ( + ) 0

let max x y = if x > y then x else y

let rgb_max (r1, g1, b1) (r2, g2, b2) = (max r1 r2, max g1 g2, max b1 b2)

let part2 (lines : string list) : int =
    List.map (parse) lines
    |> List.map (fun (_, rounds) ->
        let (r, g, b) = rounds |> List.fold_left ( rgb_max ) (0, 0, 0) in
        r * g * b
    ) |> List.fold_left ( + ) 0

let () =
    let ic = open_in file in
    let lines = In_channel.input_lines ic in
    let () = print_int (part1 lines) in
    let () = print_newline () in
    let () = print_int (part2 lines) in
    close_in ic
