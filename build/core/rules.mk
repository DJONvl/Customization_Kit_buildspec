DA: $(source-files)
	mkdir -p obj$(if $(DA_PL),_nsla,)$(if $(SW_SEC_BOOT),_swsla,)
	$(LD) -output $(OUTPUT_ELF) $(LDFLAGS) $(object-files)
	$(BIN) -bin -o $(OUTPUT_BIN) $(OUTPUT_ELF)
	$(DISASSEMBLE) -c $(OUTPUT_ELF) > $(OUTPUT_DIS)
	$(SIGN_DA)

DA_LIB: $(MAIN_OBJS)
	$(AR) -create $(OUTPUT_LIB) $(ARFLAGS) $(MAIN_OBJS)
	@echo ------ OBJ LIST --------- > $(OUTPUT_LIB_INFO)
	@$(AR) -t $(OUTPUT_LIB) >> $(OUTPUT_LIB_INFO)
	@echo ------ SYMBOL LIST ------ >> $(OUTPUT_LIB_INFO)
	@$(AR) -zs $(OUTPUT_LIB) >> $(OUTPUT_LIB_INFO)

ADD_DA_INFO:
	$(ADD_DA_INFO)
	
CUSTOM_KIT:
	-del $(CUSTOM_KIT_ZIP)
	../BootRom/tool/zip -r -9 $(CUSTOM_KIT_ZIP) $(CUSTOM_KIT_OBJ)
	../BootRom/tool/zip -u -j -9 $(CUSTOM_KIT_ZIP) ../ReleaseNote.txt

%.o : %.C
	$(CC) $(CFLAGS) -o ./$@ ./$<

%.o : %.c
	$(CC) $(CFLAGS) -o ./$@ ./$<

%.o : %.s
	$(AS) $(ASFLAGS) -o ./$@ ./$<

%.o : %.S
	$(AS) $(ASFLAGS) -o ./$@ ./$<

clean:
	-del .\\OBJ\\*.o >nul 2>&1

clean_all: clean
	-del .\\LIB\\*_DA*.a
	-del .\\LIB\\*_DA*.a.txt
	-del .\\LIB\\*_DA*.sig
	-cmd /c rd /s /q $(D_BIN_DOS) >nul 2>&1
	-cmd /c md $(D_BIN_DOS) >nul 2>&1
