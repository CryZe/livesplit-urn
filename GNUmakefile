BIN         = urn-gtk
OBJS        = urn.o urn-gtk.o bind.o $(COMPONENTS)
COMPONENTS  = $(addprefix components/, \
              urn-component.o title.o splits.o timer.o \
              prev-segment.o best-sum.o pb.o wr.o)

LIBS        = gtk+-3.0 x11 jansson
CFLAGS      += `pkg-config --cflags $(LIBS)`
LDLIBS      += `pkg-config --libs $(LIBS)`

LDLIBS	    += -L `pwd` -llivesplit_core -ldl -lutil -ldl -lrt -lpthread -lgcc_s -lc -lm -lrt -lutil

BIN_DIR     = /usr/local/bin
APP         = urn.desktop
APP_DIR     = /usr/share/applications
ICON        = urn
ICON_DIR    = /usr/share/icons/hicolor
SCHEMAS_DIR = /usr/share/glib-2.0/schemas

$(BIN): liblivesplit_core.a $(OBJS)

$(OBJS): urn-gtk.h livesplit_core.h

livesplit-core/capi/bindings/livesplit_core.h:
	(cd livesplit-core/capi/bind_gen && cargo run)

livesplit_core.h: livesplit-core/capi/bindings/livesplit_core.h
	cp livesplit-core/capi/bindings/livesplit_core.h livesplit_core.h

-include livesplit-core/target/release/liblivesplit_core_capi.d

livesplit-core/target/release/liblivesplit_core_capi.a:
	(cd livesplit-core && cargo build --release -p livesplit-core-capi)

liblivesplit_core.a: livesplit-core/target/release/liblivesplit_core_capi.a livesplit-core/capi/bindings/livesplit_core.h
	cp livesplit-core/target/release/liblivesplit_core_capi.a liblivesplit_core.a

urn-gtk.h: urn-gtk.css
	xxd --include urn-gtk.css > urn-gtk.h || (rm urn-gtk.h; false)

install:
	cp $(BIN) $(BIN_DIR)
	cp $(APP) $(APP_DIR)
	for size in 16 22 24 32 36 48 64 72 96 128 256 512; do \
	  convert $(ICON).svg -resize "$$size"x"$$size" \
	          $(ICON_DIR)/"$$size"x"$$size"/apps/$(ICON).png ; \
	done
	gtk-update-icon-cache -f -t $(ICON_DIR)
	cp urn-gtk.gschema.xml $(SCHEMAS_DIR)
	glib-compile-schemas $(SCHEMAS_DIR)
	mkdir -p /usr/share/urn/themes
	rsync -a --exclude=".*" themes /usr/share/urn

uninstall:
	rm -f $(BIN_DIR)/$(BIN)
	rm -f $(APP_DIR)/$(APP)
	rm -rf /usr/share/urn
	for size in 16 22 24 32 36 48 64 72 96 128 256 512; do \
	  rm -f $(ICON_DIR)/"$$size"x"$$size"/apps/$(ICON).png ; \
	done

remove-schema:
	rm $(SCHEMAS_DIR)/urn-gtk.gschema.xml
	glib-compile-schemas $(SCHEMAS_DIR)

rustclean:
	(cd livesplit-core && cargo clean)
	rm liblivesplit_core.a
	rm livesplit_core.h

cclean:
	rm -f $(OBJS) $(BIN) urn-gtk.h

clean: cclean rustclean
