LEU-DOBRE SIMONA 313CB

Pentru acest proiect, m-am gandit sa implementez o tabela hash prin care sunt
retinute medicamentele dintr-o farmacie si diferite informatii despre acestea, 
precum doza, pretul si o lista cu substantele continute. 
Tabela contine un vector care pointeaza catre o lista in care sunt retinute
medicamentele care incep cu o anumita litera. Acest lucru este realizat cu
ajutorul unei functii hash care intoarce codul hash pe baza caruia elementul 
este inserat.
Structurile pentru elemente sunt Telems(structura din lista de medicamente) 
care contine denumirea, doza, pretul si un pointer la o lista de substante 
continute de respectivul medicament. Structura din lista de substante este 
Tinfos, care contine denumirea substantei.

Se preiau informatiile despre elementul care se doreste inserat, se aloca
o variabila Telems si se completeaza campurile acesteia cu informatiile date. 
Aceasta variabila este data ca parametru functiei GenerareHash(), care va face introducerea elementului in tabela(in lista corespunzatoare codului hash). Mai 
intai se verifica daca elementul respectiv exista deja, acest lucru realizandu-se cu ajutorul unei functii de comparare. Daca exista, atunci va fi 
eliberat spatiul ocupat.
Altfel, se insereaza elementul in lista, apoi apelandu-se o functie de sortare 
care se ajuta de o functie de comparare, pentru a putea pastra ordinea 
lexicografica a elementelor.
Se va afisa tabela hash cu ajutorul unor functii specifice pentru afisarea 
elementelor de tip Telems si Tinfos.
In continuare, pe aceasta structura implementata am realizat functii pentru
diferite actiuni asupra informatiilor, precum afisarea medicamentelor care 
au un anumit pret, calcularea costului total pentru o lista de medicamente 
sau modoficarea pretului unui element deja existent.