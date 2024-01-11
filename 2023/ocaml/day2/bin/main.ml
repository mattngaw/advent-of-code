let file : string = "input.txt"

let part1 (lines : string list) : int =
    lines
    |> List.map (fun line ->
        let split = String.split_on_char ':' line in

        let game_str = List.nth split 0 in
        let id_str = String.sub game_str 5 (String.length game_str - 5) in
        let id = Stdlib.int_of_string id_str in

        let rounds_str = List.nth split 1 in
        let round_strs = String.split_on_char ';' rounds_str in

        round_strs
        |> List.map (fun round_str ->
            let color_strs = String.split_on_char ',' (String.trim round_str) in

            let color_counts = List.map (fun color_str ->
                let split = String.split_on_char ' ' (String.trim color_str) in
                let color = List.nth split 1 in
                let count = Stdlib.int_of_string (List.nth split 0) in
                (color, count)
            ) color_strs in

            color_counts
        )
        |> (fun rounds -> (id, rounds))
    )
    |> List.filter (fun line ->
        let (_, rounds) = line in

        rounds
        |> List.map (fun round ->
            List.map (fun (color, count) ->
                match color with
                | "red" -> count <= 12
                | "green" -> count <= 13
                | "blue" -> count <= 14
                | _ -> failwith "Unexpected color!"
            ) round
        )
        |> List.flatten
        |> List.fold_left ( && ) true
    )
    |> List.map (fun (id, _) -> id)
    |> List.fold_left ( + ) 0

let part2 (lines : string list) : int =
    ignore lines; 0

let () =
    let ic = open_in file in
    let lines = In_channel.input_lines ic in
    let () = print_int (part1 lines) in
    let () = print_newline () in
    let () = print_int (part2 lines) in
    close_in ic
