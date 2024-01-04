let file : string = "input.txt"

(* explode : string -> char list *)
let explode (s : string) : char list = 
    List.init (String.length s) (String.get s)

(* is_digit : char -> bool *)
let is_digit (c: char) : bool = 
    ('0' <= c) && (c <= '9')

(* digit_to_int : char -> int *)
let digit_to_int (c : char) : int = 
    Char.code c - Char.code '0'

(* first_last : char list -> char * char *)
let rec first_last (cs : char list) : char * char = 
    match cs with
    | [] -> failwith "Not enough elements"
    | [c] -> (c,c)
    | [c1;c2] -> (c1,c2) 
    | c::_::cs -> first_last (c::cs)

let part1 () : unit =
    let ic = open_in file in
    let lines = In_channel.input_lines ic in
    let values = List.map (fun line ->              (* For each line... *)
        let chars = explode line in                 (* Explodes the line into chars... *)
        let digits = List.filter is_digit chars in  (* Keeps only the digits *)
        let (c1,c2) = first_last digits in          (* Gets the first and last digits *)
        (digit_to_int c1 * 10) + (digit_to_int c2)  (* Converts the chars into numbers *)
    ) lines in 
    let sum = List.fold_right ( + ) values 0 in
    Printf.printf "%d\n%!" sum;
    close_in ic

let part2 () : unit =
    ()

let () =
    part1();
    part2();
