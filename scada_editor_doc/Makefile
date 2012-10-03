SRC = scada_editor.markdown

#TEMPLATE = --template=./mytemplate.tex

# article | report | book | memoir | letter | slides (transparent folie)
#DOCUMENTCLASS = --variable documentclass=memoir

MARGIN = --variable geometry=margin=2.5cm

# LaTeX \usepackage[...]{geometry}:
#   a0paper, a1paper, ..., a6paper, b0paper, b1paper, ..., b6paper,
#   letterpaper, legalpaper, executivepaper
# XeTeX:
#   'paper-format' or length-pair, e.g. 'a4', 'letter', '20cm,30cm'
#     recognized unit is 'cm', 'mm', 'bp', 'pt', 'in'
#   see /usr/share/texmf/dvips/config/config.ps
#   see /usr/share/texmf/scripts/ps2eps/ps2eps.pl
GEOMETRY = --variable geometry=a4paper

PANDOC = pandoc -S -s --toc -N --section-divs \
				 --variable lang=czech \
				 --variable date="$(shell date "+%Y-%m-%d %T %Z")" \
				 $(TEMPLATE) \
				 $(DOCUMENTCLASS) \
				 $(MARGIN) \
				 $(GEOMETRY)
#				 --html5
#				 --variable fontsize=11pt


all: html odt pdf

html:
	$(PANDOC) -o $(SRC:.markdown=.html) $(SRC)

odt:
	$(PANDOC) -o $(SRC:.markdown=.odt) $(SRC)

pdf:
	$(PANDOC) -o $(SRC:.markdown=.pdf) $(SRC)
