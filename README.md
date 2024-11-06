# Teme_Robotica
<details>
<summary> Tema 1 </summary>
  Scopul acestei teme este să simuleze un sistem interactiv a unei stații de încărcare, cu accent pe gestionarea butoanelor și utilizarea LED-urilor pentru a reflecta diferitele stadii ale încărcării.
  Un LED RGB indică disponibilitatea stației, astfel: verde pentru stație liberă și roșu pentru stație ocupată. Încărcarea este simulată prin aprinderea progresivă a patru LED-uri.
  Apăsarea butonului de start începe procesul de încărcare, în timp ce LED-urile se aprind și clipesc pentru a indica progresul. Apăsarea lungă a butonului de stop întrerupe procesul și resetează stația. La finalizarea încărcării, toate LED-urile clipesc simultan pentru a semnala finalizarea.
## Cuprins
1. Componente utilizate
2. Setup fizic
3. Funcționalitatea montajului
4. Schema electrică
## Componente utilizate
* 4 LED-uri albastre - pentru simularea procesului de încărcare
* 1 LED RGB - disponibilitatea stației de încărcare
* 2 butoane - start și stop
* 9 rezistoare (7 de 220 ohm, respectiv 2 de 1kohm)
* breadboard
* linii de legatură
## Setup fizic
<img src="https://github.com/user-attachments/assets/03c6e95c-e51d-4d86-b955-ba1a8b14b370" width="500" height="500">
<img src="https://github.com/user-attachments/assets/69b33603-3bdb-4f2d-bf8e-4e1959a7934f" width="500" height="500">

## Funcționalitatea montajului
Video cu funcționalitatea montajului:


https://github.com/user-attachments/assets/92dcd77e-fb0a-4cee-81ad-1887752aef7a


## Schema electrică
<img src="https://github.com/user-attachments/assets/e12bfd78-c0d4-4424-bd41-a04b780f3b7b" width="500" height="700">
</details>

<details>
<summary> Tema 2 </summary>
  Tema constă într-un joc de tip TypeRacer, implementat cu ajutorul Arduino Uno, menit să testeze viteza tastării. LED-ul RGB acționează ca indicator de stare: alb în repaus, clipire albă timp de 3 secunde pentru numărătoarea inversă la începutul rundei, verde pentru progres corect și roșu la greșeală. Butonul Start/Stop inițiază jocul sau îl oprește în orice moment, iar butonul de dificultate permite selectarea modului (Easy, Medium, Hard), ajustând intervalul de timp în care un cuvânt trebuie tastat.

  În timpul rundei de 30 de secunde, cuvintele generate aleator apar în terminal. Jucătorul trebuie să le tasteze corect, iar dacă reușește, un nou cuvânt apare imediat. În caz de greșeală, LED-ul devine roșu, iar corectarea cuvântului se poate face cu tasta Backspace. La finalul rundei, numărul total de cuvinte corect tastate este afișat în terminal.
## Cuprins
1. Componente utilizate
2. Setup fizic
3. Funcționalitatea montajului
4. Schema electrică
## Componente utilizate
* 1 LED RGB - pentru a semnaliza dacă cuvântul corect e scris greșit sau nu
* 2 butoane - pentru start/stop rundă și pentru selectarea dificultății
* 5 x Rezistoare (3 x 220 ohm, 2x 1000 ohm)
* breadboard
* fire de legatură
## Setup fizic

<img src="https://github.com/user-attachments/assets/96f235cd-dd37-406e-8646-45b26ae57128" width="500" height="500">
<img src="https://github.com/user-attachments/assets/43ff1afc-f5ad-4ddb-bbb6-111c06177956" width="500" height="500">


## Funcționalitatea montajului
Video cu funcționalitatea montajului:


https://github.com/user-attachments/assets/c5536781-cc0b-441e-a937-a247f7575558


## Schema electrică
<img src="https://github.com/user-attachments/assets/e61c082d-045a-41ac-85b8-b0befdfe8636" width="500" height="700">
</details>


