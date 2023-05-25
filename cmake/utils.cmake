function(init_git_submodule submodule)
    set(RECURSIVE "")
    if((ARGC GREATER 1) AND (ARGV1 STREQUAL "RECURSIVE"))
        set(RECURSIVE "--recursive")
    endif()

    # Update submodule
    find_package(Git REQUIRED)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" submodule update --init ${RECURSIVE} ${submodule}
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        RESULT_VARIABLE git_submodule_result
    )
    if(git_submodule_result EQUAL "128")
        message(WARNING "${GIT_EXECUTABLE} submodule update --init ${RECURSIVE} ${GIT_SUBMODULE} failed with error:\n ${git_submodule_result}. Current directory is an orphaned git repo.")
    elseif(NOT git_submodule_result EQUAL "0")
        message(FATAL_ERROR "${GIT_EXECUTABLE} submodule update --init ${RECURSIVE} ${GIT_SUBMODULE} failed with error:\n ${git_submodule_result}")
    endif()
endfunction()
