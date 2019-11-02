(TeX-add-style-hook
 "index"
 (lambda ()
   (TeX-add-to-alist 'LaTeX-provided-class-options
                     '(("beamer" "aspectratio=169")))
   (TeX-add-to-alist 'LaTeX-provided-package-options
                     '(("textpos" "absolute" "overlay")))
   (TeX-run-style-hooks
    "latex2e"
    "beamer"
    "beamer10"
    "tabularx"
    "graphicx"
    "eso-pic"
    "minted"
    "hyperref"
    "tcolorbox"
    "textpos")
   (TeX-add-symbols
    '("AtPagemyUpperLeft" 1))
   (LaTeX-add-environments
    "myitemize"))
 :latex)

