SUBDIRS := venc goolink rf433

		 
subdirs: 
	@for dir in $(SUBDIRS); do \
	$(MAKE) -C $$dir; \
	done 
	@cp -f venc/venc build
	@cp -f goolink/goolink build
	@cp -f rf433/rf433 build
	
clean:
	@echo "clean $(SUBDIRS)"
	@for dir in $(SUBDIRS); do \
	$(MAKE) clean -C $$dir; \
	done
	@rm -f build/*
