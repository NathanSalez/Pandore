Bienvenue sur l'application Pandore - version 1.2.1

A l'aide de cette application, vous pouvez sauvegarder vos mots de passe en sécurité.
Lors du premier lancement normal de l'application, vous devez choisir un mot de passe maître qui vous permet d'accéder aux données sensibles.
Lors des autres lancements normaux, ce mot de passe est demandé automatiquement.

Voici les différentes exécutions prévues :
./Pandore => Utilisation normale de l'application.
./Pandore -h => Affichage de l'aide utilisateur.

------------------------------------------------------------------------------------------------------------

Dans le répertoire pandoreRelease, L'arborescence suivante est présente :
- files (r) :
	- data.pass (f - alp) : contient les données de l'application. A NE PAS MODIFIER !!!
	- .help (r) :
		- data.pass (f - alp) : fichier de secours pour les données de l'application. A NE PAS MODIFIER !!!

- Pandore (f - exe) : exécutable de l'application. A NE PAS SUPPRIMER !!!

- README.txt (f - h) : fichier actuel.

Légende :
r = répertoire
f = fichier
alp = Apparition du fichier après le 1er lancement normal de l'application.
exe = exécutable
h = aide
------------------------------------------------------------------------------------------------------------

DEVLOG :	(* = version non compatible avec les précédentes.)

Version 1.0 - Original 08/10/18.

Version 1.1 * 10/08/18 :
- Correction des bugs suivants :
	- Le chemin du fichier de données non initialisé dans le constructeur, utilisé au 1er lancement.
	- Lorsque le 1er caractère du mot de passe maître chiffré était un chiffre, la clé de dechiffrage était erroné.
	- WINDOWS : Inclusion de la librairie ctime pour éviter une erreur de compilation.

Version 1.2  13/08/18 :
- Modularisation et optimisation de l'ajout de combinaisons à l'aides de méthodes statiques.
- Correction des bugs liés à l'absence de combinaisons, notamment pour la suppression et la modification.
- Fonctionnalité de modification des combinaisons modifiée : la modification s'effectue désormais sur un seul champ uniquement.
- Affichage de l'aide activé, création du README.txt

Version 1.2.1 14/08/18 :
- Correction du bug lié à la génération manuelle du mot de passe.
- Sécurisation de la procédure de chargement des fichiers.

Version 1.3 29/08/18 :
- Ajout du mode <mixed> pour les combinaisons (mixed = {site,adresse mail, mot de passe} )
- Fonctionnalité de recherche de combinaisons en fonction d'un champ ajouté.
- Réécriture partielle du code, pour l'optimiser :
    - Les return de conteneur, impliquant une copie, ont été modifiées pour qu'elles renvoient une référence.
    - L'inclusion des fichiers a été changée, File.h inclut CombinationList.h
    - Les boucle for, n'obligeant pas la présence de compteur, identifiant, utilise la forme moderne (issue de C++11)
    - Changement des enum : passage au class enum pour les combinationType et les level (facilite le static_cast)


