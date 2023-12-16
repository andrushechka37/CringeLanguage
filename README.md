# Grammatic of language(scheme of work of recursive down):

## General := `OP` +
## OP := '{' `IF` | `WHILE` | `E` '}' | `E`
## IF := 'if' '(' `E` ')' `OP`
## WHILE := 'while' '(' `E` ')' `OP`
## E := `C` {[<, >, =]  `C`} ';' <- zero or one time
## C := `T`{[ + - ] `T`}*
## T := `P`{[ * / ] `P`}*
## P := `D`{[ ^ ] `D`}*
## D := (`C`) | `S`
## S := 'sin' (`C`) | 'cos' (`C`) | `X`
## X := get_var | `N`
## N := get_number
