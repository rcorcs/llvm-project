SUBDIRS = cfg1 cfg2 cfg4 cfg5 cfg6 cfg7 cfg8 cfg11 cfg12 cfg9 cfg10 cfg13 cfg14 cfg15 unify_phi

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

test :
	$(foreach dir, $(SUBDIRS), make -C $(dir) test;)
	
	
clean :

	$(foreach dir, $(SUBDIRS), make -C $(dir) clean;)
