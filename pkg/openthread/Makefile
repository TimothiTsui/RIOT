PKG_NAME=openthread
PKG_URL=https://github.com/openthread/openthread.git
<<<<<<< HEAD
PKG_VERSION=c925b42941dc2bfc31161903e6754ff03a94489c
PKG_BUILDDIR ?= $(BINDIRBASE)/pkg/$(BOARD)/$(PKG_NAME)
OPENTHREAD_DIR = $(RIOTBASE)/pkg/openthread
=======
PKG_VERSION=thread-reference-20170716
PKG_BUILDDIR ?= $(PKGDIRBASE)/$(PKG_NAME)
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git

<<<<<<< HEAD
$(info Compile OpenThread for FTD device)
OPENTHREAD_ARGS+= --enable-cli-app=ftd --enable-application-coap
OPENTHREAD_ARGS+=  --enable-diag --enable-debug
#OPENTHREAD_ARGS+=  --enable-cert-log --enable-jam-detection

$(info $$OPENTHREAD_ARGS is [$(OPENTHREAD_ARGS)])
=======
ifneq (,$(filter openthread-cli-ftd,$(USEMODULE)))
  $(info Compile OpenThread for FTD device)
  OPENTHREAD_ARGS += --enable-cli-app=ftd
endif
ifneq (,$(filter openthread-cli-mtd,$(USEMODULE)))
  $(info Compile OpenThread for MTD device)
  OPENTHREAD_ARGS += --enable-cli-app=mtd --enable-joiner
endif
OPENTHREAD_ARGS += --enable-application-coap
CONFIG_FILE      = OPENTHREAD_PROJECT_CORE_CONFIG_FILE='\"platform_config.h\"'
$(info $$OPENTHREAD_ARGS is [${OPENTHREAD_ARGS}])
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git

.PHONY: all

<<<<<<< HEAD
CONFIG_FILE      = OPENTHREAD_PROJECT_CORE_CONFIG_FILE='\"openthread-core-platform-config.h\"'
CONFIG_FILE_PATH = $(OPENTHREAD_DIR)/contrib/

OPENTHREAD_COMMON_FLAGS = -fdata-sections -ffunction-sections -ggdb -Og -g3
OPENTHREAD_COMMON_FLAGS += -D$(CONFIG_FILE) -I$(CONFIG_FILE_PATH) 
OPENTHREAD_COMMON_FLAGS += -Wno-implicit-fallthrough
=======
OPENTHREAD_COMMON_FLAGS = -fdata-sections -ffunction-sections -Os
OPENTHREAD_COMMON_FLAGS += -Wno-implicit-fallthrough -Wno-unused-parameter
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git

all: git-download
	cd $(PKG_BUILDDIR) && PREFIX="/" ./bootstrap
	cd $(PKG_BUILDDIR) && CPP="$(CPP)" CC="$(CC)" CXX="$(CXX)"\
		OBJC="" OBJCXX="" AR="$(AR)" RANLIB="$(RANLIB)" NM="$(NM)" \
		STRIP="$(STRIP)" \
		CPPFLAGS="$(OPENTHREAD_COMMON_FLAGS) $(CFLAGS_CPU) -D$(CONFIG_FILE)" \
		CFLAGS="$(OPENTHREAD_COMMON_FLAGS) $(CFLAGS_CPU) " \
		CXXFLAGS="$(OPENTHREAD_COMMON_FLAGS) $(CFLAGS_CPU) -fno-exceptions -fno-rtti " \
		LDFLAGS="$(OPENTHREAD_COMMON_FLAGS) $(CFLAGS_CPU) -nostartfiles -specs=nano.specs \
		-specs=nosys.specs -Wl,--gc-sections -Wl,-Map=map.map " \
		./configure --disable-docs --host=$(TARGET_ARCH) --target=$(TARGET_ARCH) \
		--prefix=/ $(OPENTHREAD_ARGS)
	cd $(PKG_BUILDDIR) &&  DESTDIR=$(PKG_BUILDDIR)/output PREFIX=/ make -j4 clean install

<<<<<<< HEAD
	cp $(PKG_BUILDDIR)/output/lib/libmbedcrypto.a $(BINDIR)/libmbedcrypto.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-ftd.a $(BINDIR)/libopenthread.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-cli-ftd.a $(BINDIR)/libopenthread-cli.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-diag.a $(BINDIR)/libopenthread-diag.a
	sed -ie 's/BASE/_BASE/g' $(PKG_BUILDDIR)/output/include/openthread/types.h
=======
	cp $(PKG_BUILDDIR)/output/lib/libmbedcrypto.a ${BINDIR}/mbedcrypto.a
ifneq (,$(filter openthread-cli-ftd,$(USEMODULE)))
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-ftd.a ${BINDIR}/openthread-ftd.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-cli-ftd.a ${BINDIR}/openthread-cli-ftd.a
endif
ifneq (,$(filter openthread-cli-mtd,$(USEMODULE)))
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-mtd.a ${BINDIR}/openthread-mtd.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-cli-mtd.a ${BINDIR}/openthread-cli-mtd.a
endif
>>>>>>> branch 'master' of https://github.com/RIOT-OS/RIOT.git

include $(RIOTBASE)/pkg/pkg.mk
