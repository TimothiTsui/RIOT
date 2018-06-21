PKG_NAME=openthread
PKG_URL=https://github.com/openthread/openthread.git
PKG_VERSION=63257be510c0c89d08da5cd89cb062aae943223c
PKG_BUILDDIR ?= $(BINDIRBASE)/pkg/$(BOARD)/$(PKG_NAME)
OPENTHREAD_DIR = $(RIOTBASE)/pkg/openthread

$(info Compile OpenThread for FTD device)
OPENTHREAD_ARGS+= --enable-cli-app=ftd --enable-application-coap
OPENTHREAD_ARGS+=  --enable-diag --enable-debug
#OPENTHREAD_ARGS+=  --enable-cert-log --enable-jam-detection

$(info $$OPENTHREAD_ARGS is [$(OPENTHREAD_ARGS)])

.PHONY: all

CONFIG_FILE      = OPENTHREAD_PROJECT_CORE_CONFIG_FILE='\"openthread-core-platform-config.h\"'
CONFIG_FILE_PATH = $(OPENTHREAD_DIR)/contrib/

OPENTHREAD_COMMON_FLAGS = -fdata-sections -ffunction-sections -ggdb -Og -g3
OPENTHREAD_COMMON_FLAGS += -D$(CONFIG_FILE) -I$(CONFIG_FILE_PATH) 
OPENTHREAD_COMMON_FLAGS += -Wno-implicit-fallthrough

all: git-download
	cd $(PKG_BUILDDIR) && PREFIX="/" ./bootstrap
	cd $(PKG_BUILDDIR) && CPP="$(CPP)" CC="$(CC)" CXX="$(CXX)"\
		OBJC="" OBJCXX="" AR="$(AR)" RANLIB="$(RANLIB)" NM="$(NM)" \
		STRIP="$(STRIP)" \
		CPPFLAGS="$(OPENTHREAD_COMMON_FLAGS) $(CFLAGS_CPU) " \
		CFLAGS="$(OPENTHREAD_COMMON_FLAGS) $(CFLAGS_CPU) " \
		CXXFLAGS="$(OPENTHREAD_COMMON_FLAGS) $(CFLAGS_CPU) -fno-exceptions -fno-rtti " \
		LDFLAGS="$(OPENTHREAD_COMMON_FLAGS) $(CFLAGS_CPU) -nostartfiles -specs=nano.specs \
		-specs=nosys.specs -Wl,--gc-sections -Wl,-Map=map.map " \
		./configure --disable-docs --host=$(TARGET_ARCH) --target=$(TARGET_ARCH) \
		--prefix=/ $(OPENTHREAD_ARGS)
	cd $(PKG_BUILDDIR) &&  DESTDIR=$(PKG_BUILDDIR)/output PREFIX=/ make -j4 clean install

	cp $(PKG_BUILDDIR)/output/lib/libmbedcrypto.a $(BINDIR)/libmbedcrypto.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-ftd.a $(BINDIR)/libopenthread.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-cli-ftd.a $(BINDIR)/libopenthread-cli.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-diag.a $(BINDIR)/libopenthread-diag.a
	sed -ie 's/BASE/_BASE/g' $(PKG_BUILDDIR)/output/include/openthread/types.h

include $(RIOTBASE)/pkg/pkg.mk
