SUBDIRS =  bitonic_cfg  cost_model loop_with_single_bb nested_if2  nested_if4  nested_if_while   simple_if_else   unify_phi elseif_chain  lud_cfg nested_if  nested_if3 nested_if_asym  nested_if_while2  simple_if_else2

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

test :
	$(foreach dir, $(SUBDIRS), make -C $(dir) test;)
	
	
clean :

	$(foreach dir, $(SUBDIRS), make -C $(dir) clean;)
