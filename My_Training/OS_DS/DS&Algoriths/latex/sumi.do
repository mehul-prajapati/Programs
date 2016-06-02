#!/bin/csh
echo Make some expressions
setenv TMP /var/tmp
setenv TMPDIR /var/tmp
setenv
rm -rf sumi
latex2html -dir /var/tmp -tmp /var/tmp -no_navigation -address "" -info "" sumi.tex
mv sumi/img1.gif sumi.gif
rm -rf sumi
