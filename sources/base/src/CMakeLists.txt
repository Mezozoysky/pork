set(HEADER_FILES
    Service.hpp
    Context.hpp
    Application.hpp
    )

set(SOURCE_FILES
    Service.cpp
    Context.cpp
    Application.cpp
    )

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}"
    PREFIX ""
    FILES ${HEADER_FILES} ${SOURCE_FILES}
    )

target_sources(${PROJECT_NAME}
    PUBLIC ${HEADER_FILES}
    PRIVATE ${SOURCE_FILES}
    )
