Pour lancer les tests automatiser :
-Dans le fichier expected.txt, écrire les chaîne de caractères à envoyer, tout d'abord en binaire en utilisant la bonne méthode de conversion (correspond à la partie "message" du protocole), puis sur la ligne d'après
les 2 chaînes en caractères séparées par une tabulation.
-Brancher la carte Arduino à l'ordinateur et trouver sur quel port elle est connectée
-Vérifier que le mode "DEBUG" est désactivé dans le code Arduino (#define DEBUG 0)
-Penser à utiliser les fonctions serialPrintDatasAsBits() et serialPrintDatasAsStrings() afin d'envoyer les informations reçus sur le port série
-Avoir téléchargé et installé Processing (https://processing.org/download/)
-Lancer le fichier Lecture_Arduino.pde
-Exécuter le code (le triangle en haut à gauche)
-Repérer dans la console de processing en quelle position est le port sur lequel la carte Arduino est branchée
-Modifier si nécessaire l'indice de la ligne 10 du fichier Lecture_Arduino.pde afin qu'il corresponde au bon port
-Réexécuter Lecture_Arduino.pde
-Brancher le téléphone contenant l'application Blinkium Android et le placer devant le capteur de luminosité
-Lancez ADB une première fois (commande : adb start) afin de lancer un daemon
-Lancer le script startTests.bat
-Attendre la fin des tests
-Le fichier de sortie est le fichier result.txt(Statut;nbErreursBinaires;nbErreursCaracteres;temps), et les résultat des transmissions sont stockés dans le fichier received.txt sous le même format que le fichier expected.txt