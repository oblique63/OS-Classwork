CFLAGS=-Wall -Wextra -g -pthread

FILES = unix_processes unix_threads  unix_processes_deadlock unix_threads_deadlock

PDF = hw3a.tex

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
