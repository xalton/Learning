import pickle

nom_joueur=input("Entrez votre nom:")
print(nom_joueur)
r=0

#création du fichier scores:
s={"nom_joueur": r,}

with open('scores','ab') as fichier:  
	mon_pickler=pickle.Pickler(fichier)
	mon_pickler.dump(s)

#Afficher tout les scores:
fichier=open('scores',"rb")
contenu=fichier.read()
print("Les scores sont de :", contenu)

