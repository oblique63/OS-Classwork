CFLAGS=-Wall -Wextra -g -pthread

FILES = unix_processes_v1 unix_processes_v2 unix_processes_v3 unix_processes_v4 unix_processes_v5 \
	unix_threads

PDF = hw3.tex

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
