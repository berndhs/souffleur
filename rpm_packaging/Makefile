
# Makefile for generating RPM for Egalite / dchat
#


NAME = $(shell cat pack-name)
VERSION = $(shell cat pack-version)
COUNT = $(shell ./docount.sh)
CHANGES = $(NAME).changes

RPMBUILD = $(HOME)/rpmbuild
SOURCEDIR = $(RPMBUILD)/SOURCES

TAR_SRC = $(NAME)_$(VERSION).tar.gz
SPEC = $(NAME).spec

SRC = $(SOURCEDIR)/$(TAR_SRC) 

rpm: $(SRC) $(SPEC)
	QA_RPATHS=1 rpmbuild -ba $(SPEC) | tee build.log

fresh: NEWPACK rpm

NEWPACK:
	$(RM) pack-count

spec: $(SPEC) FORCE

$(SPEC):  spec.in pack-version Makefile
	echo -n "%define sourceversion " > $(SPEC)
	echo $(VERSION) >> $(SPEC)
	echo -n "%define relcount " >> $(SPEC)
	echo $(COUNT) >> $(SPEC)
	cat spec.in >> $(SPEC)

$(SOURCEDIR)/$(TAR_SRC): $(TAR_SRC)
	cp $(TAR_SRC) $(SOURCEDIR)/$(TAR_SRC)

FORCE:

