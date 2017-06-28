default-target:

define dos-path
$(subst  /,\\,$1)
endef

define linux-path
$(subst \,/,$(subst  \\,/,$1))
endef

define all-source-in
$(strip $(eval _:=$(patsubst %/,%,$1))
$(sort $(foreach f,$_,$(wildcard $f/*.c $f/*.C $f/*.s $f/*.S))))
endef

define parse-build-mk
$(eval LOCAL_SOURCE_FILES := ) \
$(eval include $1/build.mk ) \
$(addprefix $1/,$(LOCAL_SOURCE_FILES))
endef

define .expand-path
$(if $(wildcard $1/build.mk),$(call parse-build-mk,$1),$(call all-source-in,$1))
endef

define prepare-source-list
$(filter-out $2,$(foreach p,$1,$(call .expand-path,$p)))
endef

define c-to-obj
$(2): $(1) $(AUTOCONFIG)
	$(CC) $(CFLAGS) -o ./$$@ ./$$<
clean: clean-$(2)
clean-$(2):
	$(RM) -f $(2)
endef

define s-to-obj
$(2): $(1) $(AUTOCONFIG)
	$(CC)  -c $(ASFLAGS) -o ./$$@ ./$$<
endef

define obj-name
$(addsuffix .o,$(basename $1))
endef
define prepare-object-list
$(strip $(eval .s-src-list := $(filter %.s,$1) $(filter %.S,$1))
$(eval .c-src-list := $(filter %.c,$1) $(filter %.C,$1))
$(foreach f,$(.s-src-list),$(eval o:=$(call obj-name,$f))$(eval $(call s-to-obj,$f,$o)) $o)
$(foreach f,$(.c-src-list),$(eval o:=$(call obj-name,$f))$(eval $(call c-to-obj,$f,$o)) $o))
endef

define all-features-on
$(foreach f,$(FEATURES),$(eval FEATURES.$(f) := y))
endef
define all-features-off
$(foreach f,$(FEATURES),$(eval FEATURES.$(f) := ))
endef
define dump-feature
$(if $(FEATURES.$1),
	@echo #define FEATURE_$1 "$(FEATURES.$1)" >> $2
,)
endef
define dump-addrs
$(if $(ADDRS.$1),
	@echo #define  $1 $(ADDRS.$1) >> $2
,)
endef
define clean_obj
$(FIND) $1 -name *.o | $(XARGS) rm -f

endef