install.sh
==========

Synopsis
--------

Installation script for Disam RT SCADA system. For more information please run `install.sh help`.

Comparison to the old `isys` script
-----------------------------------

* GID and UID is automatically chosen for new users/groups
* ~dir and non-tilda dirs were divided into separate dirs (common & specific_XXX) while abandoning the `~` character

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

Changes to SCADA install data
-----------------------------

* logging capability introduced (using syslogd)

    /var/log/syslog has limited size of 2MB by default (QNX\`s syslogd itself has a hard file size limit of 2GB)

* various fixes to overcome booting issues (e.g. missing APS) and many permission fixes especially to system files
