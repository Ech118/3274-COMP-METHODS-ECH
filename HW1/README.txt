Elias C. Hadjipanayis
08/28/2026
4720185

EX1:

Adding the call to exp() introduced a new U_exp to the table. The otool -L lib dependency list was unchanged, because exp() on macOS is already in libSystem.B.dylib, which was linked regardless. c++filt did not make a difference to the symbol name since it was not mangled. The c++filt did affect the other symbol names. It made them readable.

EX2:

Note that you must still run exp with ./exp.

EX3:

My version and the cmath version produce almost exactly identical outputs.
nm tells me whether the routine is internal or external by showing a U exp or a T exp. From what I learned in my last class, I assume that T is for routines in the .text section of the machine code (internal). U is undefined.

EX4:

The output lists all of the items in my home directory. This tells me that the * character represents all items in the currently directory.

EX5:
As expected.

EX6:
	make to get the exe
	make clean to remove lib

EX7:
The size of the static lib is 15K, and the size of the shared lib is 38K (over double).


