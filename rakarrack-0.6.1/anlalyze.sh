# recherche des effects
# et des methodes
> /tmp/$$.method
for effect in `cat listeEffect.txt`
do
	grep ');' src/$effect.h | while read a method c
	do
		methode=`echo $method | cut -f 1 -d '('`
		[ x$methode == x ] && continue
		echo $methode >> /tmp/$$.method
	done
done
sort /tmp/$$.method | uniq > listeMethodes.txt
rm -f /tmp/$$.method
for effect in `cat listeEffect.txt`
do
	sort listeMethodes.txt | while read methode
	do
		grep -w $methode src/$effect.h 2>&1 > /dev/null
		echo "$effect;$methode;$?"
	done
done
