set (LIBJILL_SRC
  3rdparty/cbitset/src/bitset.c

  storage/field.c
  storage/jbitset.c
  storage/piece.c
  storage/skiplist.c
  storage/valindex.c
  storage/vallist.c
)

include_directories (3rdparty/cbitset/include)
add_library (${PROJECT_NAME} STATIC ${LIBJILL_SRC})
add_definitions(-std=gnu11)
