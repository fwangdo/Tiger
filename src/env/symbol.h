/*
 * symbol.h - Symbols (interned strings) and symbol tables.
 *
 * From Appel's "Modern Compiler Implementation in C" (Tiger book).
 *
 * Key idea:
 *   - S_Symbol interns a string so that two identical strings
 *     always map to the same pointer (cheap pointer comparison).
 *   - S_table is a scoped symbol table built on top of TAB_table
 *     (which is essentially the same hash table as EnvTable).
 *   - S_beginScope / S_endScope use a "mark" stack to undo all
 *     bindings introduced in the current scope.
 */

#ifndef SYMBOL_H
#define SYMBOL_H

/*
 * In the original Tiger book, util.h defines:
 *   typedef char *string;
 *   void *checked_malloc(int);
 *
 * We inline the typedef here so this file is self-contained.
 */
typedef char *string;


typedef struct S_symbol_ *S_Symbol;

/* Intern a string: returns the unique symbol for that name. */
S_Symbol S_Symbol(string name);

/* Retrieve the original string from a symbol. */
string S_name(S_Symbol sym);

/* ---- Scoped symbol table built on TAB_table ---- */

typedef struct TAB_table_ *S_table;

S_table  S_empty(void);                         /* create empty table       */
void     S_enter(S_table t, S_Symbol sym, void *value);  /* bind sym→value */
void    *S_look(S_table t, S_Symbol sym);        /* lookup binding           */

void     S_beginScope(S_table t);                /* push scope marker        */
void     S_endScope(S_table t);                  /* pop all bindings in scope*/

/*
 * Internal representation:
 *
 *   struct S_symbol_ { string name; S_Symbol next; };
 *
 *   - Symbols are stored in a separate hash table (the "intern table")
 *     so that S_Symbol("foo") == S_Symbol("foo") (pointer equality).
 *
 *   - S_beginScope pushes a special "marker" binding.
 *     S_endScope pops bindings until it hits the marker.
 *     This is why the linked-list (stack) structure matters:
 *     we need to undo bindings in reverse order.
 */

#endif
