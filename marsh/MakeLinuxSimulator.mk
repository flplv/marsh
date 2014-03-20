ifeq (1,${V})
	SILENT =
else
	SILENT = @  
endif

CC=g++
STRIP=strip

CFLAGS= -I src  \
		`pkg-config gtkmm-3.0 --cflags --libs` \
		-g -O0 -c -Wall
		
LDFLAGS=-I src \
		`pkg-config gtkmm-3.0 --cflags --libs` \
		-g -O0 -lpthread

PROJECT = marsh_simu
 
OBJDIR = obj
SRCFILES =
SRCDIRS = \
	platform_src/linux_simulator \
	src \
	src/helper \
	src/font_data \
	src/bitmap_data \


get_src_from_dir  = $(wildcard $1/*.cpp) $(wildcard $1/*.c)
get_dirs_from_dirspec  = $(wildcard $1)
get_src_from_dir_list = $(foreach dir, $1, $(call get_src_from_dir,$(dir)))
SRCS += $(call get_src_from_dir_list, $(SRCDIRS)) $(SRCFILES)	

__src_to = $(subst .c,$1, $(subst .cpp,$1,$2))
src_to = $(addprefix $(OBJDIR)/,$(call __src_to,$1,$2))
src_to_o = $(call src_to,.o,$1)
OBJS = $(call src_to_o,$(SRCS))

all: $(PROJECT)

$(PROJECT): buildrepo printversions $(OBJS)
	@echo
	@echo Linking $(PROJECT)
	$(SILENT)$(CC) $(OBJS) $(LDFLAGS) -o $@
	@chmod +x $(PROJECT)

$(OBJDIR)/%.o: %.cpp
	@echo Compiling $(notdir $<)
	$(SILENT)$(CC) $(CFLAGS) -c $< -o $@
	
$(OBJDIR)/%.o: %.c
	@echo Compiling $(notdir $<)
	$(SILENT)$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(PROJECT) $(OBJDIR) -Rf

printversions:
	@echo FWVersion: ${FIRWMARE_VERSION}
	@echo BuildTag: ${BUILDTAG}
	@echo SRC DIRS:
	@echo $(SRCDIRS)
	@echo SOURCES:
	@echo $(SRCS)
	@echo OBJECTS:
	@echo $(OBJS)
	@echo

buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
	for dir in $(SRCDIRS); \
	do \
		mkdir -p $(OBJDIR)/$$dir; \
	done
endef
