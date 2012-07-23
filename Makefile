all:	isv.pdf

# Don't delete intermediate files (in this case the HTML version of the word documents).
.SECONDARY:

TEX=01ge.tex 02ex.tex 03lev.tex 04num.tex 05deu.tex 06jos.tex 07jdg.tex 08rut.tex 09sa1.tex 10sa2.tex 11ki1.tex 12ki2.tex 13ch1.tex 14ch2.tex 15ezr.tex 16neh.tex 17est.tex 18job.tex 19psa.tex 20pro.tex 21ecc.tex 22sos.tex 23isa.tex 24jer.tex 25lam.tex 26eze.tex 27da.tex 28hos.tex 29joe.tex 30amo.tex 31oba.tex 32jo.tex 33mic.tex 34nah.tex 35hab.tex 36zep.tex 37hag.tex 38zec.tex 39mal.tex 40mat.tex 41mar.tex 42luk.tex 43joh.tex 44act.tex 45rom.tex 46cor1.tex 47cor2.tex 48gal.tex 49eph.tex 50php.tex 51col.tex 52th1.tex 53th2.tex 54ti1.tex 55ti2.tex 56tit.tex 57phm.tex 58heb.tex 59jam.tex 60pe1.tex 61pe2.tex 62jo1.tex 63jo2.tex 64jo3.tex 65jud.tex 66rev.tex 67end.tex

isv.pdf:	isv.tex $(TEX) isvhtml2tex
	xelatex --shell-escape isv
	xelatex --shell-escape isv
	xelatex --shell-escape isv

isvhtml2tex:	isvhtml2tex.c
	gcc -Wall -o isvhtml2tex isvhtml2tex.c

%.html:	%.doc
	/Applications/LibreOffice.app/Contents/MacOS/OOoPython.framework/Versions/2.6/bin/python  ./unoconv -f xhtml $<

%.tex:	%.html isvhtml2tex
	./isvhtml2tex $< >$@
