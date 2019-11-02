all:
	cd docs/ && \
		pdflatex -shell-escape index.tex && \
		pdflatex -shell-escape index.tex
