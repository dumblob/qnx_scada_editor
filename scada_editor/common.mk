# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.

EXTRA_SUFFIXES+=cxx cpp

ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

#===== USEFILE - the file containing the usage message for the application. 
USEFILE=$(PROJECT_ROOT)/src/Usemsg

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PROJECT_ROOT)/src

#===== POST_BUILD - extra steps to do after building the image.
define POST_BUILD
phabbind $(BUILDNAME) $(PHAB_MODULES)
cp $(PROJECT_ROOT)/src/cfgview.xml ./
endef





#===== LIBS - a space-separated list of library items to be included in the link.
LIBS+=-Bstatic xml2 -Bdynamic xml2 xml2

#===== EXTRA_LIBVPATH - a space-separated list of directories to search for library files.
EXTRA_LIBVPATH+=C:/workspace/qnx/scada_editor/libxml/lib

include $(MKFILES_ROOT)/qmacros.mk
ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif
include $(QNX_INTERNAL)

postbuild:
	$(POST_BUILD)

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))

