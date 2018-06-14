PKG_NAME=openthread
PKG_URL=https://github.com/openthread/openthread.git
PKG_VERSION=d92b424bc9cbf0b9c80a6b2561a34468928fe8e1
PKG_BUILDDIR ?= $(BINDIRBASE)/pkg/$(BOARD)/$(PKG_NAME)

$(info Compile OpenThread for FTD device)
OPENTHREAD_ARGS+= --enable-cli-app=ftd --enable-application-coap --enable-diag --enable-debug

$(info $$OPENTHREAD_ARGS is [$(OPENTHREAD_ARGS)])

.PHONY: all

OPENTHREAD_COMMON_FLAGS = -fdata-sections -ffunction-sections -ggdb -Og -g3
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
		--prefix=/ --enable-default-logging $(OPENTHREAD_ARGS)
	cd $(PKG_BUILDDIR) &&  DESTDIR=$(PKG_BUILDDIR)/output PREFIX=/ make -j4 clean install

	cp $(PKG_BUILDDIR)/output/lib/libmbedcrypto.a $(BINDIR)/libmbedcrypto.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-ftd.a $(BINDIR)/libopenthread.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-cli-ftd.a $(BINDIR)/libopenthread-cli.a
	cp $(PKG_BUILDDIR)/output/lib/libopenthread-diag.a $(BINDIR)/libopenthread-diag.a
	sed -ie 's/BASE/_BASE/g' $(PKG_BUILDDIR)/output/include/openthread/types.h

include $(RIOTBASE)/pkg/pkg.mk
