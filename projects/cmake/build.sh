#!/bin/sh

#################
# command line options
# set default values
debug="false"
mpi="false"
win="false"

help="false"
jupyter="false"
travis="false"
version="false"

all_args=$@
# parse command line arguments
while echo $1 | grep ^- > /dev/null; do
    # intercept help while parsing "-key value" pairs
    if [ "$1" = "--help" ] || [ "$1" = "-h" ]
    then
        echo '
Command line options are:
-h                              : print this help and exit.
-debug          <true|false>    : set to true to compile with debugging symbols. Defaults to false.
-mpi            <true|false>    : set to true if you want to build the MPI version. Defaults to false.
-win            <true|false>    : set to true if you are building on a Windows system. Defaults to false.
'
        exit
    fi

    # parse pairs
    eval $( echo $1 | sed 's/-//g' | tr -d '\012')=$2
    shift
    shift
done

if [ "$mpi" = "true" ] && [ "$travis" = "false" ]; then
    BUILD_DIR="build-mpi"
else
    BUILD_DIR="build"
fi

if [ "$1" = "clean" ]
then
	rm -rf ${BUILD_DIR}
else

if [ ! -d ${BUILD_DIR} ]; then
	mkdir ${BUILD_DIR}
fi

    #################
    # generate git version number
    echo "Generating version numbers in GitVersion.cpp"
    echo "#include \"GitVersion.h\"" > GitVersion.cpp
    echo "const char *build_git_sha = \"$(git describe --abbrev=6 --always)\";" >> GitVersion.cpp
    echo "const char *build_date = \"$(date)\";" >> GitVersion.cpp
    echo "const char *build_git_branch = \"$(git name-rev --name-only HEAD)\";" >> GitVersion.cpp
    
    mv GitVersion.cpp ../../src/revlanguage/utils/

	./regenerate.sh ${all_args}
	cd ${BUILD_DIR} 
	CC=gcc CXX=g++ cmake .
	make -j 4
	cd ..
fi
