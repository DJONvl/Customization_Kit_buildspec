THIS_FILE := signature.mk
SIG_PACK_FILE := sig-pack.mk

ACT  := $(if $(ACT),$(ACT),UNKNOWN)


.PHONY : FORCE

ifeq (N,$(FIRST))
include buildspec.mk

act-single : act-chips
	$(warning =========[ $(ACT) OK ]=========)
	
else

$(ACT): FORCE	
	$(if $(filter $(ACT),UNKNOWN),$(error Please specific ACT, such as 'ACT=sign' or 'ACT=pack'))
	make -f $(THIS_FILE) FIRST=N act-single	
	make -f $(THIS_FILE) FIRST=N DA_PL=yes act-single
	make -f $(THIS_FILE) FIRST=N SW_SEC_BOOT=yes act-single
		
endif

define act-bbchip
	$(warning [make $(SIG_PACK_FILE)])
	make $(ACT) -f $(SIG_PACK_FILE) BBCHIP=$1 DA_PL=$(DA_PL) SW_SEC_BOOT=$(SW_SEC_BOOT)
endef

act-chips: FORCE	
ifeq ($(ACT),sign)
	$(foreach b,$(BBCHIPS),$(call act-bbchip,$b))
else
ifeq ($(ACT),pack)
	make $(ACT) -f $(SIG_PACK_FILE) DA_PL=$(DA_PL) SW_SEC_BOOT=$(SW_SEC_BOOT)
endif
endif
	

