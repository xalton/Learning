
alphabet=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']
shift=int(input("Entrer le paramètre du shift:"))
new_alphabet=["1"]*26
print(len(alphabet))
for i in range(len(alphabet)): 
	new_alphabet[i]=alphabet[(i+shift)%26]
print("voilà, le nouvel alphabet", new_alphabet) 
	









	

