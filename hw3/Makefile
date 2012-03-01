CFLAGS=-Wall -Wextra -g

FILES = unix_processes_v1 unix_processes_v2 unix_processes_v3 unix_processes_v4
PDF = hw3.tex

all: $(FILES)

run: all
	./unix_shell
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
