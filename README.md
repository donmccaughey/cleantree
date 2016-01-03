cleantree
=========

Remove extraneous files from a directory tree.

`cleantree` walks a directory tree starting from a _root directory_ and 
removes all files **not** on a list of _files to keep_.

    Usage: cleantree [OPTIONS]... ROOT_DIR FILE_TO_KEEP...
    Remove unexpected files from a directory tree.

      -h, --help          display this help message and exit
      -V, --version       display the version number and exit


Development Status
------------------
`cleantree` is currently incomplete and unusable.


Building cleantree
------------------

`cleantree` is written in C99 and should build on most modern POSIX compatible
operating systems with a recent release of the GCC or LLVM compilers.

### From Repository Source

You must have GNU Autoconf 2.69 (or later) and Automake 1.14 (or later) installed.

        $ autoreconf -i
        $ mkdir tmp && cd tmp
        $ ../configure
        $ make
        $ sudo make install

### From Source Distribution Tarball

The `cleantree` distribution supports the standard GNU build steps.

        $ mkdir tmp && cd tmp
        $ ../configure
        $ make
        $ sudo make install

