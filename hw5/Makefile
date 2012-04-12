CFLAGS=-Wall -Wextra -g -lpthread

FILES = unix_address virtual_memory

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
