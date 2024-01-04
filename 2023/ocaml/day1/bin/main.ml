let file : string = "input.txt"

let digit_to_int (c : char) : int = 
    Char.code c - Char.code '0'

let rec first_last (cs : char list) : char * char = 
    match cs with
    | [] -> failwith "Not enough elements"
    | [c] -> (c,c)
    | [c1;c2] -> (c1,c2) 
    | c::_::cs -> first_last (c::cs)

let part1 () : unit =
    let ic = open_in file in
    let lines = In_channel.input_lines ic in
    let values = List.map (fun line ->
        let r = Str.regexp {|\d|} in
        let rec get_all_matches i = match Str.search_forward r line i with
            | i -> (Str.matched_string line) :: (get_all_matches (i+1))
            | exception Not_found -> []
        in
        let matches = get_all_matches 0 in
        let chars = List.map (fun str -> String.get str 0) matches in
        let (first, last) = first_last chars in
        let (first, last) = (digit_to_int first, digit_to_int last) in
        first * 10 + last
    ) lines in 
    let sum = List.fold_right ( + ) values 0 in
    Printf.printf "%d\n%!" sum;
    close_in ic

let part2 () : unit =
    ()

let () =
    part1();
    part2();
