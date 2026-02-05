---

```md
# Tiger Language Syntax Specification

This document defines the syntax of the Tiger programming language
as described in Appel's "Modern Compiler Implementation" (C edition).

The syntax is presented in EBNF-like notation.
This file is intended to be used as a language specification for
parser, compiler, or LLM-based reasoning.

---

## 1. Lexical Elements

### 1.1 Identifiers
```

id ::= letter (letter | digit | "_")*

```

### 1.2 Literals
```

int_lit ::= digit+
string_lit ::= " ( any_character_except_quote )* "

```

### 1.3 Keywords
```

array break do else end for function if in let nil of then to type var while

```

### 1.4 Operators and Symbols
```

* * * /  =  <>  <  <=  >  >=
      :=  .  ,  ;  :  ( )  [ ]  { }

```

---

## 2. Program Structure

```

program ::= exp

```

Tiger programs are expressions; the entire program evaluates to a value.

---

## 3. Expressions

### 3.1 General Expression
```

exp ::=
lvalue
| nil
| int_lit
| string_lit
| function_call
| op_exp
| record_exp
| seq_exp
| assign_exp
| if_exp
| while_exp
| for_exp
| break
| let_exp
| array_exp

```

---

### 3.2 L-values
```

lvalue ::=
id
| lvalue . id
| lvalue [ exp ]

```

---

### 3.3 Function Call
```

function_call ::= id ( exp_list? )
exp_list ::= exp ( , exp )*

```

---

### 3.4 Binary Operations
```

op_exp ::= exp op exp
op ::= + | - | * | / | = | <> | < | <= | > | >=

```

---

### 3.5 Record Expression
```

record_exp ::= id { field_list? }
field_list ::= field ( , field )*
field ::= id = exp

```

---

### 3.6 Sequence Expression
```

seq_exp ::= ( exp_list? )

```

---

### 3.7 Assignment
```

assign_exp ::= lvalue := exp

```

---

### 3.8 Conditional
```

if_exp ::= if exp then exp ( else exp )?

```

---

### 3.9 While Loop
```

while_exp ::= while exp do exp

```

---

### 3.10 For Loop
```

for_exp ::= for id := exp to exp do exp

```

Note:
- The loop variable is read-only inside the loop body.

---

### 3.11 Let Expression
```

let_exp ::= let dec_list in exp_list? end

```

---

### 3.12 Array Creation
```

array_exp ::= id [ exp ] of exp

```

---

## 4. Declarations

```

dec_list ::= dec+
dec ::= type_dec | var_dec | function_dec

```

---

### 4.1 Type Declaration
```

type_dec ::= type id = ty

```
```

ty ::=
id
| { type_fields? }
| array of id

```
```

type_fields ::= type_field ( , type_field )*
type_field ::= id : id

```

---

### 4.2 Variable Declaration
```

var_dec ::= var id ( : id )? := exp

```

---

### 4.3 Function Declaration
```

function_dec ::= function id ( type_fields? ) ( : id )? = exp

```

---

## 5. Operator Precedence (High → Low)

1. `* /`
2. `+ -`
3. `< <= > >=`
4. `= <>`
5. `:=`

All binary operators are left-associative.

---

## 6. Notes for Implementation

- Tiger is expression-oriented: most constructs return values.
- `break` is only valid inside loops.
- `nil` is compatible with record types.
- Type equivalence is name-based, not structural.
- Recursive type and function declarations are allowed.

---

End of specification.
```

---

