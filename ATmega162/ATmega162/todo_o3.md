# 1.
Test test_ram funksjonen at den fortsatt fungerer etter vi la inn nand-gates.
Om alt funker burde vi også få riktig chip select til ADCen

# 2.
For MAX156 timing:
${t_{conversion} = \frac{9 \cdot N \cdot 2}{f_{clock}}}$ 

Regner med at vi vil basere klokka vår på ATmegaen, så F_clock = 4915200 Hz

N er antall signaler man konverterer

Vi vil vel få 7 konversjoner? (thumbstick x og y axis, thumbstick knapp og to slidere+knapper)

Må sette mode til 0, kobla til ground for å få 8 analoge input som blir sammenlignet med ground. 

Vil dermed få at ${t_{cconversion} = 14.6484e^{-6}s = 14.6 \mu s}$

Chat mener det ikke er mulig å sende ut f_cpu gjennom SCK, kun f_cpu/2, f_cpu/4, f_cpu/8 osv...

Han mener vi må bruke CKOUT funksjon på PB0 for å få ut eksakt klokkefrekvens. (omprgoramere en fuse?)


