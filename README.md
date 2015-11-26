cleantree
=========

Remove extraneous files from a directory tree.


Building cleantree
------------------

### From Repository Source

You must have GNU Autoconf and Automake installed.

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

