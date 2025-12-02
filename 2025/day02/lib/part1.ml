(*
 * 1) Split input on commas to get list of ranges
 * 2) Map list of ranges to list of lists of numbers?
 * 3) Filter list of lists of numbers to list of lists of invalid numbers?
 * 4) Flatten list of lists to just list of invalid numbers
 * 5) Reduce list of numbers to a sum
 *)

let solve (filename : string) : int =

  In_channel.with_open_text filename In_channel.input_all
  |> String.trim
  |> String.split_on_char ','
  |> List.map (fun r -> 
      let split = String.split_on_char '-' r in
      let (lo, hi) = match split with
        | [l; h] -> (Stdlib.int_of_string l, Stdlib.int_of_string h)
        | _ -> failwith "expected exactly two"
      in
      let rec build l h =
        match (l = h) with
        | false -> l :: (build (l+1) h)
        | true -> [h]
      in
      build lo hi)
  |> List.flatten
  |> List.map Stdlib.string_of_int
  |> List.filter (fun s ->
      let len = String.length s in
      let even = len mod 2 = 0 in
      let half1 = String.sub s 0 (len/2) in
      let half2 = String.sub s (len/2) (len/2) in
      even && half1 = half2)
  |> List.map Stdlib.int_of_string
  |> List.fold_left (+) 0
