
#!/bin/sh
echo Make the complexity pages
latex complexity.tex
dvips -o complexity.ps complexity
rm -f *.log *.aux
