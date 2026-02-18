/*
 * symbol.c - Symbols (interned strings) and scoped symbol tables.
 *
 * From Appel's "Modern Compiler Implementation in C" (Tiger book).
 */

/*
 * In the original Tiger book, these come from util.h:
 *   typedef char *string;
 *   void *checked_malloc(int len);
 *
 * And from table.h:
 *   TAB_table  TAB_empty(void);
 *   void       TAB_enter(TAB_table t, void *key, void *value);
 *   void      *TAB_look (TAB_table t, void *key);
 *   void      *TAB_pop  (TAB_table t);
 *
 * We include symbol.h which inlines the string typedef.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol.h"

/* ====================================================================
 * 1. Symbol interning
 * ====================================================================
 *
 * A symbol is just a (name, next) pair stored in a hash table.
 * "next" chains symbols that hash to the same bucket.
 *
 * The point: after interning, string comparison becomes pointer
 * comparison — O(1) instead of O(n).
 */

struct S_symbol_ {
  string name;
  S_Symbol next;
};

static S_Symbol mksymbol(string name, S_Symbol next) {
  S_Symbol s = malloc(sizeof(*s));
  s->name = name;
  s->next = next;
  return s;
}

#define SIZE 109   /* same prime as EnvTable */

static S_Symbol hashtable[SIZE];

static unsigned int hash(char *s) {
  unsigned int h = 0;
  for (; *s; s++)
    h = h * 65599 + *s;
  return h;
}

/*
 * S_Symbol("foo"):
 *   - Hash the name, walk the chain looking for a match.
 *   - If found, return existing symbol (interned).
 *   - If not, create a new symbol and prepend to chain.
 */
S_Symbol S_Symbol(string name) {
  int index = hash(name) % SIZE;
  S_Symbol syms = hashtable[index], sym;

  for (sym = syms; sym; sym = sym->next)
    if (strcmp(sym->name, name) == 0)
      return sym;                        /* already interned */

  sym = mksymbol(name, syms);
  hashtable[index] = sym;
  return sym;
}

string S_name(S_Symbol s) {
  return s->name;
}

/* ====================================================================
 * 2. Scoped symbol table
 * ====================================================================
 *
 * Built on top of TAB_table (the hash table with linked-list chaining).
 *
 * Key mechanism for scoping:
 *   - S_beginScope() inserts a special "marker" symbol into the table.
 *   - S_enter() binds symbols as usual.
 *   - S_endScope() pops bindings until it hits the marker.
 *
 * This is why linked-list chaining matters: we need LIFO (stack)
 * semantics to undo scope in reverse order.
 */

static struct S_symbol_ marksym = { "<mark>", NULL };

S_table S_empty(void) {
  return TAB_empty();
}

void S_enter(S_table t, S_Symbol sym, void *value) {
  TAB_enter(t, sym, value);
}

void *S_look(S_table t, S_Symbol sym) {
  return TAB_look(t, sym);
}

/*
 * S_beginScope: push a marker binding so we know where this scope starts.
 */
void S_beginScope(S_table t) {
  S_enter(t, &marksym, NULL);
}

/*
 * S_endScope: pop all bindings until we hit the marker.
 *
 * This calls TAB_pop repeatedly.  Each TAB_pop removes the most recent
 * binding for a given key — exactly the stack-like behavior we discussed.
 */
void S_endScope(S_table t) {
  S_Symbol s;
  do {
    s = TAB_pop(t);
  } while (s != &marksym);
}
