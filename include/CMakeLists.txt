add_library(Board board.c board.h)

add_library(Globals globals.c globals.h)

add_library(GridCell gridcell.c gridcell.h)

add_library(Hashset hashset.c hashset.h)

add_library(Menu menu.c menu.h)

add_library(MenuItem menuitem.c menuitem.h)

add_library(Piece piece.c piece.h)


set(CUSTOM_LIBS Board Globals GridCell Hashset Menu MenuItem Piece)
set(RAYLIB_DEPENDENT_LIBS Board Globals GridCell Menu MenuItem Piece)

# Add include dir for custom libs
foreach(lib ${CUSTOM_LIBS})
    target_include_directories(${lib} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endforeach()

# raylib needs to be linked to these libs since they depend on it
foreach(lib ${RAYLIB_DEPENDENT_LIBS})
    target_link_libraries(${lib} PRIVATE raylib)
endforeach()

#    For some reason if the PARENT_SCOPE flag is used CUSTOM_LIBS doesn't set the include dirs properly above therefore CUSTOM_LIBS has to be re-set with
#    the PARENT_SCOPE flag before returning to the parent file 
set(CUSTOM_LIBS Board Globals GridCell Hashset Menu MenuItem Piece PARENT_SCOPE)


