# Grigorescu Stefan D., grupa 131
# Tema2 Laborator LFA: generarea cuvintelor de o lungime data ale unei gramatici regulate

epsilon = 'ε'
sageata = '→'
cod_epsilon = "Îµ"
cod_sageata = "â†’"
len_cod_sageata = len(cod_sageata)

def afis_productie(dictionar, cheie):
    rezultat = cheie + ":"
    lista = []
    for elem in dictionar[cheie]:
        if str(elem) == cod_epsilon:
            lista.append(epsilon)
        else:
            lista.append(elem)

    rezultat = rezultat + str(lista)
    print(rezultat)


productii = dict([])

f = open("Regex.in", "r")
linie = f.readline()
linie = linie[1:]
for caracter in linie:
    if caracter >= 'A' and caracter <= 'Z':
        productii[caracter] = []

alfabet = f.readline()

# print("\nProductiile se vor modifica astfel:")
linie = f.readline()
while linie != "":
    cheie = linie[0]

    poz_sageata = linie.find(cod_sageata)
    valoare = linie[poz_sageata + len_cod_sageata:]
    valoare = valoare.strip()    # !OBS: pt ε, valoarea stocata in dictionar va fi cod_epsilon

    productii[cheie].append(valoare)
    #afis_productie(productii, cheie)

    linie = f.readline()

f.close()

print("\nProductiile sunt: ")
for neterminal in productii:
    afis_productie(productii, neterminal)

simbol_start = 'S'
lungime_cuvinte = int(input("\nAlegeti lungimea cuvintelor: "))
# lungime_cuvinte = 3
cuvant = [' ']*lungime_cuvinte

def afis_cuvant(cuvant):
    rezultat = ""
    for litera in cuvant:
        rezultat = rezultat + litera
    print(rezultat)

def backtracking(neterminal, poz):
    for dreapta_productiei in productii[neterminal]:
        if dreapta_productiei == cod_epsilon:   # daca productia merge in epsilon
            if poz == lungime_cuvinte:    # daca aveam deja un cuv de lungimea cautata
                afis_cuvant(cuvant)
        else:
            if poz < lungime_cuvinte:
                cuvant[poz] = dreapta_productiei[0]
                if len(dreapta_productiei) == 2:  # daca productia merge in terminal si neterminal
                    backtracking(dreapta_productiei[1], poz+1)
                elif poz == lungime_cuvinte - 1:  # daca productia merge in terminal si cuv are lungimea cautata
                    afis_cuvant(cuvant)

print("Cuvintele de lungime " + str(lungime_cuvinte) + " acceptate de gramatica sunt: ")
backtracking(simbol_start, 0)



# Exemplul de la laborator
#
# N = {S, A}
# Σ = {a, b, c}
# S → aS
# S → bA
# A → ε
# A → cA
