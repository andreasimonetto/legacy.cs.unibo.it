include config.mk

#main target aaos.core.umps
all: kernel tape0.umps disk0.umps

tape0.umps: 
	$(MKDEV) -t $@ $(KERNEL_CORE)

disk0.umps:
	$(MKDEV) -d $@

$(TARGETS): %: 
	$(MAKE) -C $(SRCDIR) $@
	ln -sf $(SRCDIR)/$@.core.umps $(KERNEL_CORE)
	ln -sf $(SRCDIR)/$@.stab.umps $(KERNEL_STAB)

clean:
	$(MAKE) -C $(SRCDIR) clean
	$(MAKE) -C $(TESTDIR) clean
	-rm -f $(KERNEL_CORE) $(KERNEL_STAB)
	-rm -f $(LIBRARY) p1test
	-rm -f term?.umps printer?.umps disk?.umps tape?.umps

distclean: clean
	-rm -f *.core.umps *.stab.umps

dist: distclean
	cd .. && tar czf $(DISTNAME)-$(VERSION).tar.gz $(DISTNAME)/

doc:
	(echo PROJECT_NUMBER = $(VERSION) && cat doc/Doxyfile.template) | doxygen -

.PHONY: all dist clean distclean doc $(TARGETS)
