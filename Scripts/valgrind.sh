#!/bin/bash

make debug

valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes\
	--track-origins=yes --suppressions=/usr/share/glib-2.0/valgrind/glib.supp -s\
	./programa-debug Dataset_Fase1 teste.txt
