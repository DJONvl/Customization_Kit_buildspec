include make/base.mk
include buildspec.mk
ifeq ($(MAKECMDGOALS),clean)
clean:
	-$(RM) -rf bin bin_nsla bin_swsla
	-$(RM) -rf temp
	-$(foreach b, $(wildcard ./arch/*),$(call clean_obj,$b) )
	-$(foreach b, $(wildcard ./platform/*),$(call clean_obj,$b) )
	-$(foreach b, $(wildcard ./custom/*),$(call clean_obj,$b) )
	-$(call clean_obj,"./boot") 
	-$(call clean_obj,"./kernel") 
	-$(call clean_obj,"./lib") 
	-$(call clean_obj,"./driver") 
else
	
ifeq ($(MAKECMDGOALS),custom_kit)
	-include make/custom_kit.mk
else
BUILD_TYPE	?= CUSTOM
LOG2 		?= build_log.txt

ifeq ($(LOG2),on)
	include make/main.mk
else
$(MAKECMDGOALS) : recursive-make
recursive-make:
	@echo "called_by=$(CALLED_BY)"
	@make  BBCHIPS="$(BBCHIP)" BUILD_TYPE=$(BUILD_TYPE) DA_PL=$(DA_PL) LOG2=on 2>&1 | $(TEE) $(LOG2)
	
endif

endif

endif

