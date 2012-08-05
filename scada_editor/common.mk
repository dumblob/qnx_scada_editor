# This is an automatically generated record.
# The area between QNX Internal Start and QNX Internal End is controlled by
# the QNX IDE properties.

EXTRA_SUFFIXES+=cxx cpp

ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

ifndef QNX_INTERNAL
QNX_INTERNAL=$(PROJECT_ROOT)/.qnx_internal.mk
endif

#===== USEFILE - the file containing the usage message for the application.
USEFILE=$(PROJECT_ROOT)/src/Usemsg

#===== EXTRA_SRCVPATH - a space-separated list of directories to search for source files.
EXTRA_SRCVPATH+=$(PROJECT_ROOT)/src $(PROJECT_ROOT)/libxml/include/libxml2/

#===== LIBS - a space-separated list of library items to be included in the link.
LIBS+=-Bdynamic xml2 xml2

#===== LDFLAGS - add the flags to the linker command line.
LDFLAGS+=-Wl,-rpath,./ -Wl,-rpath,$(PROJECT_ROOT)/libxml/lib/

#===== CCFLAGS - add the flags to the C compiler command line.
#CCFLAGS+=-O2

#===== EXTRA_LIBVPATH - a space-separated list of directories to search for library files.
EXTRA_LIBVPATH+=$(PROJECT_ROOT)/libxml/lib
#EXTRA_LIBVPATH+=C:/workspace/qnx/scada_editor/libxml/lib

include $(MKFILES_ROOT)/qmacros.mk
include $(QNX_INTERNAL)

#===== POST_BUILD - extra steps to do after building the image.
define POST_BUILD
	phabbind $(BUILDNAME) $(PHAB_MODULES)
	@#cp $(PROJECT_ROOT)/src/cfgview.xml ./
	cp $(PROJECT_ROOT)/libxml/lib/libxml2.so.10 ./
endef

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
