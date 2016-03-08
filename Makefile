define program-deps
$1: LDFLAGS += $($1_ldflags)
$1: $($1_objs)
	$$(LINK.cpp) $(OUTPUT_OPTION) $$@ $$^

$($1_objs): Makefile

endef

define to-deps
$(addprefix .,$(addsuffix .d,$1))
endef

define make-depend
$(COMPILE.cpp) -MM -MF $(call to-deps,$1) -MP -MT $2 $1
endef

prefix ?= /usr
programs = qfind qselect
qfind_objs = qfind.o dir.o match.o
qselect_objs = qselect.o match.o term.o

CXXFLAGS += -std=c++14 -Wall -Werror

all: $(programs)

install: $(programs)
	mkdir -p $(prefix)/bin
	install $(programs) $(prefix)/bin

uninstall:
	$(foreach p,$(programs),$(RM) $(prefix)/bin/$(p))

clean:
	$(RM) $(programs) *.o .*.d

.PHONY: all clean install uninstall

$(foreach prog,$(programs), $(eval $(call program-deps,$(prog))))

%.o: %.cpp
	@$(call make-depend,$<,$@)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

ifneq "$(MAKECMDGOALS)" "clean"
-include $(call to-deps,$(wildcard *.cpp))
endif
