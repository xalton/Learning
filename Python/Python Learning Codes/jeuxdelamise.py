import random
J=0
O=0

def question(joueur, ordi,J,O):
	if joueur== 'oui' and ordi=='non':
		J=J-1
		O=O+2
	elif joueur=='oui' and ordi== 'oui':
		J=J+1
		O=O+1
	elif ordi=='oui' and joueur=='non':
		O=O-1
		J=J+2
	else:
		J=J+0
		O=O+0
	return J,O 

while J<10 and O<10:
	try:
		joueur= input("Tu mises ou pas ?:") 
		if joueur != "oui" and joueur != "non":
			raise NameError("pas le bon mots")
	except NameError: 
		print("Vous n'avez pas introduit la bonne réponse, rejouer!")
	joueur= input("Tu mises ou pas ?:") 
	ordinateur=random.randrange(10)
	if 0<=ordinateur<=5:
		ordi='non'
		print("L'ordi a répondu:", ordi)
	else:
		ordi='oui'
		print("L'ordi a répondu:", ordi)
	J,O=question(joueur,ordi,J,O)
	print("le score est de {} pour le joueur et {} pour l'ordinateur".format(J,O))

if J>=10:
	print("Félicitation, tu as gagné")
elif O>=10:
	print("Désolé, tu as perdu comme une merde ")
else:
	print("La partie n'est pas terminé")
	
