CFLAGS=-Wall -Wextra -g -lpthread -lm

FILES = unix_sort

PDF = hw7.tex

all: $(FILES)

run:
	./unix_sort 18 0 && ./unix_sort 18 1 && ./unix_sort 18 2 && ./unix_sort 18 3

tex:
	pdflatex -shell-escape $(PDF)
	rm -f *.log *.aux *.pyg

latex: tex
pdf: tex

clean:
	rm -f $(FILES)
	rm -rf *.dSYM

with-clean: clean
	make
