CUSTOM_KIT_ZIP 	:= ./Customization_Kit_buildspec.zip

subdir 		:= Raphael-da/
ZIP 		:= zip.exe
ZIP_FLAGS 	:=-r -9 -q -A
custom-kits-common  := $(subdir)arch/arm/arch_arm_lib_init.o	\
				$(subdir)arch/arm/arch_arm_lib_loop.o	\
				$(subdir)arch/arm/include/*				\
				$(subdir)arch/arm/ld/*					\
				$(subdir)arch/arm/*.mk					\
				$(subdir)arch/arm/makefile				\
				$(subdir)bin/*					 		\
				$(subdir)bin_nsla/*					 	\
				$(subdir)driver/driver_lib_init.o		\
				$(subdir)driver/driver_lib_loop.o		\
				$(subdir)driver/UFS/*.h					\
				$(subdir)boot/boot_lib_init.o			\
				$(subdir)boot/boot_lib_loop.o			\
				$(subdir)boot/*.h						\
				$(subdir)boot/checksum/*.h				\
				$(subdir)boot/dev_interface/*.h			\
				$(subdir)boot/partition/*.h				\
				$(subdir)boot/loader_file/*.h			\
				$(subdir)boot/sparse/*.h				\
				$(subdir)boot/Makefile					\
				$(subdir)boot/security_export/*.h		\
				$(subdir)custom/* 						\
				$(subdir)include/*						\
				$(subdir)kernel/kernel_lib_init.o		\
				$(subdir)kernel/kernel_lib_loop.o		\
				$(subdir)kernel/Makefile				\
				$(subdir)lib/lib.o						\
				$(subdir)lib/Makefile					\
				$(subdir)make/*.mk						\
				$(subdir)project/*.mk					\
				$(subdir)temp/*							\
				$(subdir)custom/*						\
				$(subdir)platform/epp					\
                $(subdir)Makefile 						\
				$(subdir)buildspec.mk					\
				$(subdir)Rules.make						\
				$(subdir)make.exe						\
						build/tools					\
						../Raphael-inc				\
						../Tools				\
			
define remove-elf-etc
	$(warning Remove all unnecessary files.)
	-$(RM) -f bin/MTK_$1_DA.bin/*.elf bin/MTK_$1_DA.bin/*.dis bin/MTK_$1_DA.bin/*.map bin/MTK_AllInOne_DA.bin temp/$1/da_loop_hash.o
	
endef
#	-$(RM) -f bin_nsla/MTK_$1_DA.bin/*.elf bin_nsla/MTK_$1_DA.bin/*.dis bin_nsla/MTK_$1_DA.bin/*.map
#	-$(RM) -f bin_swsla/MTK_$1_DA.bin/*.elf bin_swsla/MTK_$1_DA.bin/*.dis bin_swsla/MTK_$1_DA.bin/*.map	




define zip-custom-kit-chip
	echo $(CUSTOM_KIT_ZIP)
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP) 	$(subdir)platform/$(subst _E1,,$1)/$1_platform_init.o
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$(subst _E1,,$1)/$1_platform_loop.o
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/dainfo/*
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/inc/*
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/sec_lib/*
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/emi/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/gpt_timer/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/irq/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/low_power/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/mmc/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/nand/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/partition/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/pll/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/pmic/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/power/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/rtc_bbpu/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/uart/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/usb/*.h
	cd .. && $(ZIP) $(ZIP_FLAGS) $(CUSTOM_KIT_ZIP)	$(subdir)platform/$1/dev/wdt/*.h
	
endef

define BUILD_TYPE_modify
	$(MV)	makefile  makefile_temp	-f
	$(SED) -e "s/DEBUG/CUSTOM/" makefile_temp	> makefile 
	
endef

define BUILD_TYPE_resume
	-$(CHMOD)	777 makefile_temp
	-$(MV)	makefile_temp  makefile -f
	
endef

custom_kit: clean_zip  custom_common_zip $(BBCHIP) 
	@echo "CUSTOM_KIT over"

clean_zip:	
#	-$(RM) $(CUSTOM_KIT_ZIP)
	@echo "clean CUSTOM_KIT zip OK"
	
custom_common_zip:
	$(call BUILD_TYPE_modify)
	
	cd .. && $(ZIP) -r -9  $(CUSTOM_KIT_ZIP)  $(custom-kits-common) 

	$(call BUILD_TYPE_resume)
	
$(BBCHIP):
	$(call remove-elf-etc,$@)
	$(call zip-custom-kit-chip,$@)