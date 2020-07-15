open Sedlexing.Utf8;
open Tokens;

let alpha = [%sedlex.regexp? 'a'..'z' | 'A'..'Z'];
let digit = [%sedlex.regexp? '0'..'9'];
let int = [%sedlex.regexp? Plus(digit)];
let whitespace = [%sedlex.regexp? ' ' | '\t' | '\n'];

// TODO: is rgb(255 255 255/0) valid?

// TODO: keyword characters, like . and , also escape like '*'
let keyword = [%sedlex.regexp? (alpha, Star(alpha | digit | '-'))];

let read_char = buf => {
  let char =
    switch%sedlex (buf) {
    | "<" => LOWER_THAN
    | ">" => GREATER_THAN
    | "'" => QUOTE
    | "&&" => DOUBLE_AMPERSAND
    | "||" => DOUBLE_BAR
    | "|" => BAR
    | "[" => LEFT_BRACKET
    | "]" => RIGHT_BRACKET
    | "*" => ASTERISK
    | "+" => PLUS
    | "?" => QUESTION_MARK
    | "{" => LEFT_BRACE
    | "}" => RIGHT_BRACE
    | "," => COMMA
    | "#" => HASH
    | "!" => EXCLAMATION_POINT
    | "(" => LEFT_PARENS
    | ")" => RIGHT_PARENS
    | "/" => SLASH
    | _ => failwith("Unexpected character")
    };
  let _ = lexeme(buf);
  char;
};
let rec read = buf =>
  switch%sedlex (buf) {
  | eof => EOF
  | int => INT(lexeme(buf) |> int_of_string)
  | whitespace =>
    let _ = lexeme(buf);
    read(buf);
  | keyword => KEYWORD(lexeme(buf))
  | ("'", any, "'") =>
    let chars = lexeme(buf);
    KEYWORD(String.sub(chars, 1, String.length(chars) - 2));
  | _ => read_char(buf)
  };
