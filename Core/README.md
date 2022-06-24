<h1 style="text-align: center;">Traffic Tracker Core</h1>

Blocco principale del sistema back-end di Traffic Tracker.

Programma per la gestione delle telecamere e per la raccolta, elaborazione e registrazione dei dati delle telecamere.

---
Programma sviluppato durante il corso di **Multimedia App and Internet of Things** dell' **Universita' degli Studi di Genova** 

---

## 1 - Installazione

### 1.1 - Dipendenze:
- opencv-cuda
- cxxopts
- make
- mysql++

Per installare le dipendenze:
#### Arch:
```bash
sudo pacman -S opencv-cuda cxxopts make mysql++
```
#### Compilazione da sorgente:
 - opencv con supporto a CUDA
 
 [https://github.com/opencv/opencv](https://github.com/opencv/opencv)
 
 <p style="color: red;">Se si compila dai sorgenti opencv e' necessario installare separatamente CUDA e cuDNN seguendo le istruzioni di nVidia e facendo particolare attenzione alle versioni compatibili ed a diversi bug che potrebbero renderle ugualmente incompatibili</p>

 - cxxopts

 [https://github.com/jarro2783/cxxopts](https://github.com/jarro2783/cxxopts)

 - make

 [https://github.com/wkusnierczyk/make](https://github.com/wkusnierczyk/make)

 - mysql++

 [https://github.com/tangentsoft/mysqlpp](https://github.com/tangentsoft/mysqlpp)

##### NOTA:
In alcuni sistemi l'include path potrebbe essere leggermente diverso, in questo caso e' necessario intervenire manualmente sul makefile, sugli #include o direttamente sul filesistem. Per esempio per poter compilare correttamente sul mio sistema ho dovuto effettuare le seguenti modifiche:
 
```bash
cp /usr/include/opencv4/opencv2 /usr/include/opencv2
echo \#include \<mysql++/mysql++.h\> >> /usr/include
echo \#include \<mysql/mysql.h\> >> /usr/include
```
### 1.2 - Compilazione

#### Ogni sistema:

```bash
make
```
Il compilatore di default per questo progetto e' *clang++*. Se si preferisce usarne uno diverso e' possibile sostituirlo modificando il **Makefile** 

## 2 - Utilizzo

```bash
bin/TrafficTracker
```
<p style="color: red;"><b>E' STRETTAMENTE NECESSARIO lanciare il programma dalla directory TrafficTracker/Core. Il programma non implementa ancora un installazione che posiziona i file di config in un path prestabilito (es: .config) e di conseguena si andrebbero a rompere i path relativi</b></p>

## 3 - Note Finali
Per una questione di dimensione del file (nell'ordine del Giga) in questo repository non e' presente il file dei pesi della rete neurale. E' comunque possibile sostituirli con qualsiasi altra rete **yolov4** aggiungendo i file *yolov4.cfg* e *yolov4.weights* alla cartella *dnn_model* rinominandoli **traffictracker.cfg** e **traffictracker.weighrs**. Nel caso in cui la rete non segua il numero e l'ordine delle classi che yolov4 riconosce di default e' inoltre necessario modificare il file *classes.txt* in modo da rappresentare la nuova rete.

---

Buona Fortuna nel provare a compilare questo repo **:D**
