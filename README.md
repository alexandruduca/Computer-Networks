# Connect 4

Cerinta: Simulati jocul Connect Four intr-o aplicatie client/server. Fiecare client se conecteaza 
la server unde acesta va stabili jucatorul care face prima mutare si culoare cu care va 
juca fiecare. Serverul are rolul de a tine scorul, jocul putand fi jucat pe mai multe 
reprise, precum si rolul de a afisa fiecarui jucator gridul fiecarui jucator, dupa ce se 
efectueaza o mutare.

Resurse suplimentare: https://en.wikipedia.org/wiki/Connect_Four

Pentru a rula proiectul: make s1 make c1

Proiectul a fost realizat in cadrul disciplinei "Retele de calculatoare" si simuleaza jocul Connect Four, folosind o aplicatie de tip server/client. Serverul are responsabilitatea de a asigna culoarea fiecarui jucator (rosu sau galben) si de a asigura buna desfasurare a jocului. Serverul valideaza mutarile jucatorilor, tine scorul si ofera posibilitatea jucatorilor de a juca din nou dupa ce un joc se termina. Clientul este responsabil de a afisa tabla de joc si de a trimite catre server o mutare pentru a fi validata si ulterior actualizata in tabla.
