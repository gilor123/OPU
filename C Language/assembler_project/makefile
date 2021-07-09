assembler: main.o assembler_compilation.o functions_to_support_output_files.o general_supporting_functions.o linked_list.o data_structures.o
	gcc -g -ansi -Wall -pedantic main.o assembler_compilation.o functions_to_support_output_files.o general_supporting_functions.o linked_list.o data_structures.o -o assembler
main.o: main.c assembler_compilation.o assembler_compilation.h
	gcc -c -g -ansi -Wall -pedantic main.c -o main.o
assembler_compilation.o: assembler_compilation.c
	gcc -c -g -ansi -Wall -pedantic assembler_compilation.c -o assembler_compilation.o
functions_to_support_output_files.o: functions_to_support_output_files.c functions_to_support_output_files.h general_supporting_functions.h
	gcc -c -g -ansi -Wall -pedantic functions_to_support_output_files.c -o functions_to_support_output_files.o
general_supporting_functions.o: general_supporting_functions.c general_supporting_functions.h enums_constants.h linked_list.h data_structures.h
	gcc -c -g -ansi -Wall -pedantic general_supporting_functions.c -o general_supporting_functions.o
linked_list.o: linked_list.c linked_list.h
	gcc -c -g -ansi -Wall -pedantic linked_list.c -o linked_list.o
data_structures.o: data_structures.c data_structures.h enums_constants.h linked_list.h
	gcc -c -g -ansi -Wall -pedantic data_structures.c -o data_structures.o




