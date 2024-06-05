# Примеры использования языка:
## Большая часть информации о проекте указана в [моем x86 бэкенде для этого языка](https://github.com/andrushechka37/Compiler)
## Факториал 5:
```
{
    x = 5;
    $factorial;
    print(answer);

    $factorial {

        {
            if (x = 1;)
            answer = 1;
            ret;
        }

        {x = x - 1;
        $factorial;

        x = x + 1;
        answer = answer * x;
        ret;}
    };
}

```

## Решение квадратного уравнения:
```
{
    a = 1;
    b = 3;
    c = 1;
    d = 0;

    $discriminant;

    $answer;

    $discriminant {
        d = b * b - 4 * a * c;
    };

    $answer {
        {
            if (a = 0;) 

            {
                if (b ! 0;)
                x = -1 * b / c;
                print(x);
                ret;
            }

            {
                if (b = 0;) 

                {
                    if (c = 0;)
                    print(9999);
                    ret;
                }
            }

            {print(-1111);

            ret;}
        }

    

        {
            if (d > 0;)

            x = (-1 * b + sqrt(d)) / (2*a);
            y = (-1 * b - sqrt(d)) / (2*a);

            print(x);
            print(y);
            ret;
        }

        {
            if (d = 0;)
            x = -1 * b / (2*a);
            print(x);
            ret;
        }

        {
            if (d < 0;)
            print(-1111);
            ret;
        }
    };
}
```



# Grammatic of language(scheme of work of recursive down):

### get_program := `get_single_part_of_program` 'END_OF_PROGRAM'
### get_single_part_of_program := '{' `get_single_part_of_program` '}'* | `get_operators`
### get_operators := `IF` *| `WHILE` *| `get_expression` * | '{' `get_single_part_of_program` '}' *
### IF := 'if' '(' `get_expression` ')' `get_single_part_of_program`
### WHILE := 'while' '(' `get_expression` ')' `get_single_part_of_program`
### get_expression := `get_subexpression` ';'| `get_subexpression` {[<, >, =]  `get_subexpression` ';'}
### get_subexpression := `get_mul_or_div`{[ + - ] `get_mul_or_div`}*
### get_mul_or_div := `get_pow`{[ * / ] `get_pow`}*
### get_pow := `get_bracket`{[ ^ ] `get_bracket`}*
### get_bracket := (`get_subexpression`) | `get_long_op`
### get_long_op := 'sin' (`get_subexpression`) | 'cos' (`get_subexpression`) | `get_variable`
### get_variable := get_var | `get_number`
### get_number := get_number