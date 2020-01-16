docs-acod:
	cd docs/artintoscience/ && \
		pdflatex -shell-escape index.tex && \
		pdflatex -shell-escape index.tex

docs-isaca:
	cd docs/isaca/ && \
		pdflatex -shell-escape index.tex && \
		pdflatex -shell-escape index.tex
