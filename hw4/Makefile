CFLAGS=-Wall -Wextra -g -lpthread

FILES = unix_synchronized unix_unsynchronized

PDF = hw4.tex

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
