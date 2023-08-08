set(HEADER_FILES
    App.hpp
    Ball.hpp
    Paddle.hpp
    Field.hpp
    Score.hpp
    )
set(SOURCE_FILES
    main.cpp

    App.cpp
    Ball.cpp
    Paddle.cpp
    Field.cpp
    Score.cpp
    )

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}
    PREFIX pong-sources
    FILES ${HEADER_FILES} ${SOURCE_FILES}
    )

target_sources(${PROJECT_NAME}
    PRIVATE ${HEADER_FILES} ${SOURCE_FILES}
    )
