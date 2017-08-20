# Spell Corrector
A spell corrector program written in C, intented to work with many languages. I honestly haven't done much testing with this, but in theory it *should* work.

## How to Compile
To compile the program, a C/C++ compiler is needed. I personally use GCC, and the instructions in this section are specifically for that compiler. Note that to compile the libraries, the additional flag `-std=c99` may be needed.  
  
Firstly, to compile the program into an executable file, run the following command in a terminal:  
  
`$ gcc -o spellcorrector src/main.c src/corrector.c src/dictionary.c src/edits.c`

This will create a file called `spellcorrector`, with no extention, that's already labelled as an executable by your \*NIX or \*BSD system.

## How to Use
Once the program is compiled and the executable is created, 3 text files are required for its proper execution:

* An input file, which should contain the words to correct. These words can be arranged in paragraphs, separated by new lines, etc.
* An output file, which should be writable. Doesn't matter if it's empty or not, since the program will automatically delete its contents before writing into it.
* A dictionary file, which should contain words separated either by spaces or new lines. These words will form a Universe of words that are considered properly spelled. Ideally, all these words will be lowercase.
* An optional alphabet file, which should contain a string of all the letters in the alphabet of the language that the input file is in. For example, in Spanish the alphabet would look like: `aábcdeéfghiíjklmnoópqrstuúvwxyz`. If this file is not passed as an argument, defaultAlphabet.txt will be used instead.

The names of these files are entirely up to the user to define. Once these files are in the main directory of the program, to execute it, the following command should be ran in the terminal:  
`$ ./spellcorrector input_file.txt output_file.txt dictionary.txt alphabet.txt`  
Note that the `alphabet.txt` argument is optional.  
  
Once done, the incorrectly spelled words should appear in `output_file.txt`, along with some correct word suggestions if there were any.
