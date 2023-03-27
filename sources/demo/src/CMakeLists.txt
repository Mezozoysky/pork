set(HEADER_FILES
    App.hpp
    )
set(SOURCE_FILES
    main.cpp

    App.cpp
    )

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}
    PREFIX demo-sources
    FILES ${HEADER_FILES} ${SOURCE_FILES}
    )

target_sources(${PROJECT_NAME}
    PRIVATE ${HEADER_FILES} ${SOURCE_FILES}
    )
