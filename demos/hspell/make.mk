HSPELLOBJS=demos/hspell/word.o demos/hspell/hspell.o

-include $(patsubst %.o, %.d, $(HSPELLOBJS))

demos/hspell/hspell:  $(HSPELLOBJS)
	mkdir -p `dirname $@`
	$(CXX) -o $@  $(HSPELLOBJS) $(DEMOLDFLAGS) -lann
