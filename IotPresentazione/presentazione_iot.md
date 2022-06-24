---
title: Traffic Tracker
author: 
 - Andrea Valenzano
 - Davide Giovanetti
institute: Universita' degli Studi di Genova
date: 2021/2022

theme: Hannover
colortheme: dracula

output:
  bookdown::pdf_book:
    base_format: rmarkdown::beamer_presentation
    latex_engine: xelatex
    toc: false
    slide_level: 2

campact-title: false
---

# Introduzione

## Un problema $\Rightarrow$ Una soluzione
Per lo svolgimento del nostro progetto abbiamo preso in considerazione uno dei piu' noti problemi delle grandi citta': il traffico. 

Abbiamo quindi deciso di creare un sistema Iot in grado di monitorare lo scorrimento in diverse strade della citta' e di poter comunicare tramite un applicazione e in tempo reale ogni anomalia che puo' portare alla creazione di code ed ingorghi.

## Componenti del sistema
 - **Server di riconoscimento e tracciamento**

    Riceve l'input video e lo elabora scrivendo il riscutato delle elaborazioni sul Database
 - **Database**

    Strumento usato per la persistenza dei dati nel sistemas
 - **Server http/php** 

    Fornisce un interfaccia al Database per l'applicazione Android (JSON)
 - **Applicazione** 

    Interfaccia Utente

# Tracciamento e riconoscimento

## Tech Stack

![](tracker.png)

## Tracciamento e Riconoscimento

**Funzionalita'**

 - Interfaccia di gestione delle telecamere

 - Ricezione del dello stream video

 - Tracciamento e Riconoscimento dei veicoli

 - Manipolazione ed elaborazione dei dati delle telecamere

# Database / Server PHP

## Tech Stack

![](server.png)

## Storing e comunicazione

**Funzionalita'**

 - Conservare i dati delle rilevazioni

 - Conservare dati relativi alle telecamere

 - Comunicazione dei dati all'App

# Applicazione

## Tech Stack

![](app.png)

## Applicazione Android

**Funzionalita'**

 - Fornire un interfaccia grafica all'utente

 - Accesso rapido ed intuitivo ai dati