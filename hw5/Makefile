CFLAGS=-Wall -Wextra -g -lm

FILES = unix_address virtual_memory virtual_memory_to_csv

PDF = hw5.tex

all: $(FILES)

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
