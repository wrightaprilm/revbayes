Building RevBayes with cmake
############################

You build RevBayes by running a build script appropriate for your
operating system:

Example for a Linux system:
$ ./build.sh

The build script produces the RevBayes console executable 'rb'

Additional Options
===================  =============================================
-debug     <true|false>  compile debugging symbols. Default=false
-help      <true|false>  update the help database. Default=false
-mpi       <true|false>  build the MPI version 'rb-mpi'. Default=false
-version   <true|false>  update GitVersion.cpp, Default=false
-win       <true|false>  build for a Windows system. Default=false

To avoid using the parallel version of make, you will need to
modify the build.sh script by deleting the -j 4 flag:
$ make -j 4


