install.sh
==========

Synopsis
--------

Installation script for Disam RT SCADA system. For more information please run `install.sh help`.

Examples of installation
------------------------

workstation

    ./install.sh install -m ms1 -s install_data/ -p proj33 -i 33 -t tech33

measurement station

    ./install.sh install -s install_data/ -p proj33 -i 1 -t tech33

uninstall

    ./install.sh uninstall -b /backup_20131224-101933

Comparison to the old `isys` script
-----------------------------------

*   GID and UID is automatically chosen for new users/groups
*   ~dir and non-tilda dirs were divided into separate dirs (common & specific_XXX) while abandoning the `~` character

    many different files/dirs were moved between common and specific_XXX dirs

*   no symlinks are replaced, because they are (and have to be!) relative in SRCPATH
*   assume no weird files like *.RLnk with special undefined meaning are present in SRCPATH
*   QNX licenses are not installed any more
*   permissions are already present SRCPATH => no need for:

    chown -R root:root "/usr/disam/"
    chown -R root:root "/usr/include/drt"
    chmod -R ugo+r "/usr/include/drt"
    etc.

*   assume dirs

    home/~ms_admin/.ph/
    home/~ps_admin/.ph/

    are the same as

    home/ms/.ph/
    home/ps/.ph/

*   much more (local and remote installation, different autodetections, uninstall possibility, logging, etc.)

Changes to SCADA install data
-----------------------------

*   logging capability introduced (using syslogd)

    /var/log/syslog has limited size of 2MB by default (QNX\`s syslogd itself has a hard file size limit of 2GB)

*   various fixes to overcome booting issues (e.g. missing APS) and many permission fixes especially to system files
