include_path=$(shell echo '-I' ; echo -n $(search_path) | sed -e 's%:%/doc -I %g' ; echo '/doc')



doc/libann.info: doc/libann.texinfo
	@$(RM) $@
	mkdir -p doc
	$(MAKEINFO) -I doc $(include_path) $< -o $@

%.dvi: %.texinfo
	@$(RM) $@
	$(RM) $*.cps $*.log $*.aux $*.toc $*.tp $*.vr $*.fn $*.cp $*.ky $*.pg
	mkdir -p `dirname $@`
	cd `dirname $@` && $(TEXI2DVI) `basename $<`


doc/vers.texinfo: $(top_srcdir)/vp
	@$(RM) $@
	mkdir -p `dirname $@`
	echo '@macro version' >> $@
	awk -F= '/VERSION/{print $$2}' $< >> $@
	echo '@end macro' >> $@

# Not a very good dependency rule.  It doesn't allow for nested includes
doc/libann.d: doc/libann.texinfo
	@$(RM) $@
	mkdir -p `dirname $@`
	echo -n "doc/libann.dvi doc/libann.info: $< " \
	  | sed -e "s%$(top_srcdir)/*%%g" >> $@
	sed -n '/@include *\([^@]*\)/s//doc\/\1 \\/gp' $<  >> $@
	echo >> $@

-include doc/libann.d


.PHONY: clean
clean::
	$(RM) doc/*.info
	$(RM) doc/*.aux
	$(RM) doc/*.cp
	$(RM) doc/*.cps
	$(RM) doc/*.dvi
	$(RM) doc/*.fn
	$(RM) doc/*.ky
	$(RM) doc/*.log
	$(RM) doc/*.pg
	$(RM) doc/*.toc
	$(RM) doc/*.tp
	$(RM) doc/*.vr
