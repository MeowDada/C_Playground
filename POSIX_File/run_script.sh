#!/bin/bash

POSIX_CP_EXAMPLE_BASENAME=posix_cp_example
POSIX_CP_EXAMPLE_PROG=./posix_cp_example
POSIX_CP_EXAMPLE_BUILD_CMD=build_posix_cp_example
POSIX_CP_EXAMPLE_COPY_SRC=lorem.txt
POSIX_CP_EXAMPLE_COPY_TARGET=lorem_copy.txt

CREATE_NO_WRITEFILE_BASENAME=create_no_writfile
CREATE_NO_WRITEFILE_PROG=./create_no_writefile
CREATE_NO_WRITEFILE_BUILD_CMD=build_create_no_writefile
CREATE_NO_WRITEFILE_FILENAME=no_writefile.txt

POSIX_OPEN_EXAMPLE_BASENAME=posix_open_example
POSIX_OPEN_EXAMPLE_PROG=./posix_open_example
POSIX_OPEN_EXAMPLE_BUILD_CMD=build_posix_open_example

FCNTL_DUP_FD_BASENAME=fcntl_dup_fd
FCNTL_DUP_FD_PROG=./fcntl_dup_fd
FCNTL_DUP_FD_BUILD_CMD=build_posix_fcntl_dup_fd

PROGAM_LIST=($POSIX_CP_EXAMPLE_BASENAME $CREATE_NO_WRITEFILE_BASENAME $FCNTL_DUP_FD_BASENAME)

function containsElement () {
    local e match="$1"
    shift
    for e; do [[ "$e" == "$match" ]] && return 0; done
    return 1
}

function run_posix_cp_example () {
    if [ -f "$POSIX_CP_EXAMPLE_PROG" ]; then
        $POSIX_CP_EXAMPLE_PROG $POSIX_CP_EXAMPLE_COPY_SRC $POSIX_CP_EXAMPLE_COPY_TARGET
        ls -lh $POSIX_CP_EXAMPLE_COPY_SRC $POSIX_CP_EXAMPLE_COPY_TARGET
    else
        echo "File: ${POSIX_CP_EXAMPLE_PROG} does not exist."
        echo "Automatically try to rebuild it..."
        make ${POSIX_CP_EXAMPLE_BUILD_CMD}
        if [ $? == 0 ]; then
            echo "Build ${POSIX_CP_EXAMPLE_PROG} successfully!"
            $POSIX_CP_EXAMPLE_PROG $POSIX_CP_EXAMPLE_COPY_SRC $POSIX_CP_EXAMPLE_COPY_TARGET
            ls -lh $POSIX_CP_EXAMPLE_COPY_SRC $POSIX_CP_EXAMPLE_COPY_TARGET
        else
            echo "Build ${POSIX_CP_EXAMPLE_PROG} failed with some errors..."
        fi
    fi
}

function run_create_no_writefile () {
    if [ -f "$CREATE_NO_WRITEFILE_PROG" ]; then
        $CREATE_NO_WRITEFILE_PROG $CREATE_NO_WRITEFILE_FILENAME
        ls -lh $CREATE_NO_WRITEFILE_FILENAME
    else
        echo "File: ${CREATE_NO_WRITEFILE_PROG} does not exist."
        echo "Automatically try to rebuild it..."
        make ${CREATE_NO_WRITEFILE_BUILD_CMD}
        if [ $? == 0 ]; then
            echo "Build ${CREATE_NO_WRITEFILE_PROG} successfully!"
            $CREATE_NO_WRITEFILE_PROG $CREATE_NO_WRITEFILE_FILENAME
            ls -lh $CREATE_NO_WRITEFILE_FILENAME
        else
            echo "Build ${CREATE_NO_WRITEFILE_PROG} failed with some errors..."
        fi
    fi
}

function run_posix_open_example () {
    if [ -f "$POSIX_OPEN_EXAMPLE_PROG" ]; then
        $POSIX_OPEN_EXAMPLE_PROG
    else
        echo "File: ${POSIX_OPEN_EXAMPLE_PROG} does not exist."
        echo "Automatically try to rebuild it..."
        make ${POSIX_OPEN_EXAMPLE_BUILD_CMD}
        if [ $? == 0 ]; then
            echo "Build ${POSIX_OPEN_EXAMPLE_PROG} successfully!"
            $POSIX_OPEN_EXAMPLE_PROG
        else
            echo "Build ${POSIX_OPEN_EXAMPLE_PROG} failed with some errors..."
        fi
    fi
}

function run_fcntl_dup_fd () {
    if [ -f "$FCNTL_DUP_FD_PROG" ]; then
        $FCNTL_DUP_FD_PROG
    else
        echo "File: ${FCNTL_DUP_FD_PROG} does not exist."
        echo "Automatically try to rebuild it..."
        make ${FCNTL_DUP_FD_BUILD_CMD}
        if [ $? == 0 ]; then
            echo "Build ${FCNTL_DUP_FD_PROG} successfully!"
            $FCNTL_DUP_FD_PROG
        else
            echo "Build ${FCNTL_DUP_FD_PROG} failed with some errors..."
        fi
    fi
}

function run_program_if_valid_cmd () {
    if [ $(containsElement "$1" ${PROGRAM_LIST[@]})==0 ]; then
        run_$1
    else
        echo "invalid command"
    fi
}

function determine_program_to_run () {
    for var in "$@"
    do
        run_program_if_valid_cmd $var
    done
}

determine_program_to_run $@