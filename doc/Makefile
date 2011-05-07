# This makefile is for convenience use only.
# It should not be called by any other makefile

all:
	if [ ! -h doc ] ; then 	ln -s . doc ; fi 
	$(MAKE) -f doc/doc.mk doc/libann.info
	$(MAKE) -f doc/doc.mk doc/libann.dvi

include doc.mk
