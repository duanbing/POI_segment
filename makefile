
objects= test_main.o max_prob_seg.o max_match_seg.o dict.o kspath.o

seg:$(objects)
	g++ -o seg $(objects) -lm -g

kspath.o : kspath.h util.h
test_main.o : max_match_seg.h max_prob_seg.h
max_match_seg.o : max_match_seg.h util.h dict.h  
max_prob_seg.o : max_prob_seg.h util.h dict.h kspath.h
dict.o : dict.h util.h
.PHONY : clean
clean :
	rm seg $(objects)

