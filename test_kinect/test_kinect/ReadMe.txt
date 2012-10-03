========================================================================
   APPLICATION CONSOLE : Vue d'ensemble du projet test_kinect
========================================================================

AppWizard a créé cette application test_kinect pour vous.

Ce fichier contient un résumé des éléments contenus dans chaque fichier qui
constitue votre application test_kinect.


test_kinect.vcxproj
    Il s'agit du fichier projet principal pour les projets VC++ générés à l'aide d'un Assistant Application.
    Il contient des informations relatives à la version de Visual C++ qui a généré le fichier, ainsi que des informations sur les plateformes, 
    configurations et fonctionnalités du projet que vous avez sélectionnées dans l'Assistant Application.

test_kinect.vcxproj.filters
    Il s'agit du fichier de filtres pour les projets VC++ générés à l'aide d'un Assistant Application. 
    Il contient des informations sur l'association entre les fichiers de votre projet et les filtres. Cette association est utilisée dans l'IDE pour 
    afficher le regroupement des fichiers qui ont des extensions similares sous un nœud spécifique (par exemple, les fichiers ".cpp" sont associés au 
    filtre "Fichiers sources").

test_kinect.cpp
    Il s'agit du fichier source principal de l'application.

/////////////////////////////////////////////////////////////////////////////
Autres fichiers standard :

StdAfx.h, StdAfx.cpp
    Ces fichiers sont utilisés pour générer un fichier d'en-tête précompilé (PCH) nommé test_kinect.pch et un fichier de types précompilés 
    nommé StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Autres remarques :

AppWizard utilise des commentaires "TODO:" pour indiquer les parties du code source où vous devrez ajouter ou modifier du code.

/////////////////////////////////////////////////////////////////////////////

/!\ IMPORTANT : COMMENT INCLURE LES HEADERS NECESSAIRES AU FONCTIONNEMENT DE LA KINECT /!\

En fait, c'est un résumé de la doc, et comme la vie n'est pas rose et que ça n'a pas compilé tout de suite, voilà ce qu'il faut faire

Comme j'ai la flemme de réécrire le chemin à chaque fois, on va dire qu'à chaque fois que j'écris "[...]\v1.0 Beta2" c'est
le dossier d'installation du SDK (par exemple, chez moi c'est C:\Program Files\Microsoft SDKs\Kinect\v1.0 Beta2)

- Vérifier que la variable d'environnement KINECTSDK_DIR est correctement spécifiée (sa valeur doit être [...]\v1.0 Beta2)

- Inclure dans le fichier stdafx.h :
	- windows.h (avec un petit ou un grand 'W', ça ne change rien)
	- Les fichiers utiles du dossier [...]\v1.0 Beta2\inc\ :
		- MSR_NuiApi.h qui regroupe tous les headers NUI
		- MSRKinectAudio.h pour l'API Audio

- Ajouter des dossiers au projet d'en-têtes et de bibliothèques : clic-droit sur le projet>Propriétés>Propriétés de configuration>Répertoires VC++ :
	- Vérifier qu'on est dans la configuration "Toutes les configurations" (en haut à gauche de la fenêtre), histoire que l'ajout se fasse en Debug ET en Releases
	- Répertoires Include : Vérifier que l'option "Hériter des paramètres par défaut du parent ou du projet" est cochée
	et ajouter le chemin vers le dossier include ([...]\v1.0 Beta2\inc)
	- Répertoires de bibliothèques : Vérifier que l'option "Hériter des paramètres par défaut du parent ou du projet" est cochée
	et ajouter le chemin vers le dossier lib ([...]\v1.0 Beta2\lib)

Si ça ne compile toujours pas, penser à fermer MVSC++ et le relancer
