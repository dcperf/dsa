flexio test application program
=============================================

use flexio sdk for basic programming

Requirements
-------------

* FlexIO SDK

Build
-------------
From top folder.
1. Build with system flexio sdk
`$ ./build.sh --clean`

2. Build with specified flexio sdk
```
$ export FLEXIO_HOME=$FLEXIO_DIR/flexio-sdk/install/opt/mellanox/flexio
$ export OMPI_HOME=$OPENMPI_DIR/ompi/install
$ ./build.sh --clean --flexio_include_path $FLEXIO_HOME/include --flexio_lib_path $FLEXIO_HOME/lib --mpi $OMPI_HOME
```

Available options:

* `--flexio_include_path`:  Path to FlexIO headers.
* `--flexio_lib_path`:      Path to FlexIO libraries.
* `--mpi`:                  Path to MPI installation dir.
* `--debug`:                Build in debug mode.
* `--clean`:                Clean build folder.
* `--install`:              Install after build.
* `--prefix`:               Installation path.
* `-v`:                     Build with verbose output.

Run
-----------
From top folder.
```
$ ./builddir/app/wod_app

OR

$ $MPI_HOME/bin/mpirun --tag-output -n 4 --host hgx-isr1-033:2,hgx-isr1-042:2 -x UCX_WARN_UNUSED_ENV_VARS=n -x LD_LIBRARY_PATH=$OMPI_HOME/lib:$FLEXIO_HOME/lib $PWD/builddir/app/wod_app

```
