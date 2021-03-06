#ifndef JILL_VALLIST_LENGTH_PREFIXED_INCLUDED
#define JILL_VALLIST_LENGTH_PREFIXED_INCLUDED

/*  vallist type */
#define JILL_VALLIST_LENGTH_PREFIXED 1

/*  subtypes */
#define JILL_VALLIST_LENGTH_PREFIXED_I8 1
#define JILL_VALLIST_LENGTH_PREFIXED_U8 2
#define JILL_VALLIST_LENGTH_PREFIXED_I16 3
#define JILL_VALLIST_LENGTH_PREFIXED_U16 4
#define JILL_VALLIST_LENGTH_PREFIXED_I32 5
#define JILL_VALLIST_LENGTH_PREFIXED_U32 6

/*  custom commands */

/*  following two commands accepts 'int *' as argument, which is null or has
    value that greater than or equal to zero*/
/*  grow length buffer */
#define JILL_VALLIST_LENGTH_PREFIXED_CMD_GROWLB 0
/*  extend value buffer's size */
#define JILL_VALLIST_LENGTH_PREFIXED_CMD_EXTENDV 1

extern struct jill_vallist_base jill_vallist_length_prefixed_base;

#endif
