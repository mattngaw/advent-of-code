let file : string = "input.txt"

(* explode :  *)
let explode (s : string) : char list = 
    List.init (String.length s) (String.get s)

let is_digit (c: char) : bool = 
    ('0' <= c) && (c <= '9')

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
        let chars = explode line in
        let digits = List.filter is_digit chars in
        let (c1,c2) = first_last digits in
        (digit_to_int c1 * 10) + (digit_to_int c2)
    ) lines in 
    let sum = List.fold_right ( + ) values 0 in
    Printf.printf "%d\n%!" sum;
    close_in ic

let part2 () : unit =
    ()

let () =
    part1();
    part2();
