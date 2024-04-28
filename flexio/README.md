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
$ ./build.sh --clean --flexio_include_path $FLEXIO_HOME/include --flexio_lib_path $FLEXIO_HOME/lib
```

Available options:

* `--flexio_include_path`:  Path to FlexIO headers.
* `--flexio_lib_path`:      Path to FlexIO libraries.
* `--debug`:                Build in debug mode.
* `--clean`:                Clean build folder.
* `--install`:              Install after build.
* `--prefix`:               Installation path.
* `-v`:                     Build with verbose output.

Run
-----------
From top folder.
```
./builddir/app/wod_app
```
