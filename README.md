# Grammatic of language(scheme of work of recursive down):

### get_program := `get_operator` +
### get_operator := '{' `IF` | `WHILE` | `get_expression` '}' | `get_expression`
### IF := 'if' '(' `get_expression` ')' `get_operator`
### WHILE := 'while' '(' `get_expression` ')' `get_operator`
### get_expression := `get_subexpression` {[<, >, =]  `get_subexpression`} ';' <- zero or one time
### get_subexpression := `get_mul_or_div`{[ + - ] `get_mul_or_div`}*
### get_mul_or_div := `get_pow`{[ * / ] `get_pow`}*
### get_pow := `get_bracket`{[ ^ ] `get_bracket`}*
### get_bracket := (`get_subexpression`) | `get_long_op`
### get_long_op := 'sin' (`get_subexpression`) | 'cos' (`get_subexpression`) | `get_variable`
### get_variable := get_var | `get_number`
### get_number := get_number
