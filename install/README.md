install.sh
==========

Synopsis
--------

Installation script for Disam RT SCADA system. For more information please run `install.sh help`.

Comparison to the old `isys` script
-----------------------------------

* GID and UID is automatically chosen for new users/groups
* ~dir and non-tilda dirs divided into separate directories (common & specific_???) while abandoning the `~` character

    many different files/dirs were moved between common and specific_??? dirs

* no symlinks are replace, because they are (and have to be!) relative in SRCPATH
* assume no weird files like *.RLnk with special undefined meaning are present in SRCPATH
* QNX licenses are not installed any more
* permissions are already present SRCPATH => no need for:

    chown -R root:root "/usr/disam/"
    chown -R root:root "/usr/include/drt"
    chmod -R ugo+r "/usr/include/drt"
    etc.

* assume dirs

    home/~ms_admin/.ph/
    home/~ps_admin/.ph/

    are the same as

    home/ms/.ph/
    home/ps/.ph/

* much more (local and remote installation, different autodetections, uninstall possibility, logging, etc.)
