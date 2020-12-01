function(set_project_warnings project_name)
  set(CLANG_WARNINGS
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wunused
    -Wredundant-decls
    -Wstrict-prototypes
    -Wmissing-prototypes
    -Wmissing-declarations
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
    -Wdouble-promotion
    -Wformat=2
    -Wimplicit-fallthrough
    -Wstrict-overflow=5
  )


  set(GCC_WARNINGS
    ${CLANG_WARNINGS}
    #    -Warith-conversion # Doesnt work with old gcc
    -Wformat-overflow=2
    -Wmisleading-indentation
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
  )

  if (CMAKE_C_COMPILER_ID MATCHES ".*Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
  elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
  endif()

  target_compile_options(${project_name} INTERFACE ${PROJECT_WARNINGS})

endfunction()
