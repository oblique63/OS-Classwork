CFLAGS=-Wall -Wextra -g -lpthread -lm

FILES = unix_sort unix_sort_auto

PDF = hw7.tex

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
