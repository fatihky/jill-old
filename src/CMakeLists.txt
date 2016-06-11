set (LIBJILL_SRC
  3rdparty/cbitset/src/bitset.c

  core/field.c
  core/piece.c
  core/skiplist.c
  # core/valindex.c
  core/vallist.c

  vallist/fixed.c
  vallist/len_prefixed.c
  vallist/jbitset.c

  utils/stopwatch.c
)

include_directories (3rdparty/cbitset/include)
add_library (${PROJECT_NAME} STATIC ${LIBJILL_SRC})
add_definitions(-std=gnu11)
add_definitions(-g)
