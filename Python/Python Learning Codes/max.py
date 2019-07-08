cours1=input("note /20 pour math:")
math= float(cours1)
cours2=input("note /20 pour quant:")
quant=float(cours2)
cours3=input("note /20 pour cdi:")
cdi=float(cours3)
cours4=input("note /20 pour spectro:")
spectro=float(cours4)
cours5=input("note /20 pour astro:")
astro=float(cours5)
m=1/35*(math*10+quant*10+cdi*10+spectro*2.5+astro*2.5)
if m<10:
    print(m,"/20","Tu pue la merde")
elif m==10 or 10<m<14:
    print(m,"/20","bof")
else:
    print(m,"/20","BG")    
