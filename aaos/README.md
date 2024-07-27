# Double A Operating System.

## Cos'è A.A.O.S.?

Un sistema operativo con microkernel sviluppato sulla base delle specifiche [AMIKaya](https://www.cs.unibo.it/~morsiani/lso-07-amikaya-eval-p1.pdf) per il corso di Laboratorio di Sistemi Operativi presso l'Università degli Studi di Bologna, scritto da Andrea Simonetto™.

## Compilazione
	
Questa versione del S.O. è progettata per funzionare sull'architettura simulata [uMPS](https://github.com/tjonjic/umps). Oltre al simulatore, il processo di compilazione necessita del cross-compiler per architettura MIPS e del relativo linker.

Tutte le costanti per la compilazione si trovano nel file `config.mk`. Infine lo GNU make e (seppur opzionale ai fini della compilazione) [Doxygen](https://www.doxygen.nl/) per generare la documentazione.

- Per compilare phase1:
```bash
$ make p1test
```

- Per compilare phase2:
```bash
$ make
```

## Esecuzione

Per eseguire il kernel compilato, lanciare:
```bash
$ umps
```
	
## Scrivere codice per il S.O.
	
Il reference delle funzioni si trova in `doc/html`. Per ora, l'unico modo per scrivere codice è modificare la constante `TARGETS` nel file `config.mk` aggiungendo all'elenco il nome di un programma (ad esempio *user_kernel*) e salvando il sorgente del programma nella cartella `src/` con lo stesso nome seguito dall'estensione `.c` (nell'esempio: `user_kernel.c`).

## Tipi di dati astratti

Ho inserito un modulo per i tipi di dato astratto. Nell'attuale implementazione, i tipi di dato sono definiti tramite la loro documentazione (per il funzionamento ed i prototipi) e da una macro del tipo `DEF_ADT_*`, che contiene il codice delle funzioni sul tipo di dato e le (eventuali) strutture dati interne. Il tipo di dato è un parametro della macro. La documentazione è considerata parte integrante dell'ADT, in quanto descrive i meccanismi di generazione dei prototipi delle funzioni oltre al loro significato. I prototipi delle funzioni sull'ADT sono sempre diversi non solo in quanto parametrizzati nel tipo, ma anche per la loro segnatura.

Il problema è che in C non esiste l'overloading, quindi ogni funzione che manipola l'ADT deve avere un nome univoco, parametrizzato da un altro parametro fondamentale della macro: il Dominio. Ecco un esempio di utilizzo
di un ADT vettore:

Leggo dalla documentazione che le funzioni dell'ADT vettore sono:
```C
T get_<Dominio>(int i);
void set_<Dominio>(int i, T val);
```
e la macro per istanziarle è:
```C
DEF_ADT_VECT(T, Domain, Size)
```
posso scrivere un programma:
```C
DEF_ADT_VECT(double, dbl, 128)
```
che genererà:
```C
double get_dbl(int i) { /* code */... }
void set_dbl(int i, double val) { /* code */ ... }
```

La stesura di un nuovo ADT richiede uno sforzo sostanzialmente uguale a quello di scrivere un template in C++ o simili, tenendo presente l'importanza della documentazione (senza non si potrebbe sapere nemmeno quali funzioni quell'ADT incapsula!!). Nell'esempio sopra:

```C
#define DEF_ADT_VECT(T, Domain, Size) \
\
T vect_##Domain[Size]; \
\
T get_##Domain(int i) { \
	return vect_##Domain[i]; \
} \
\
void set_##Domain(int i, T val) { \
	vect_##Domain[i] = val; \
}
```

Tuttavia il debug è reso difficile dal fatto che l'espansione delle macro non è visibile allo sviluppatore/tester di un ADT. Per questo ho creato uno script (nella cartella test/) che provoca l'espansione delle macro `DEF_ADT_*` all'interno dei file `*_test.unexp.c`, salvando il risultato nel corrispondente `_test.c`. Inoltre un *Makefile* compila tutti i file `*_test.c`. Nell'esempio:

```C
/* vector_test.unexp.c */
DEF_ADT_VECT(double, dbl, 128)

int main() ...
/* END OF FILE */

// After the call to make vector_test.c, this file will be created:

/* vector_test.unexp.c */
	double vect_dbl[128];
	double get_dbl(int i) {
		return vect_dbl[i];
	}
	
	void set_dbl(int i, double val) {
		vect_dbl[i] = val;
	}

int main() ...
/* END OF FILE */
```

Al momento sono definiti gli ADT Queue e SMM (Static Memory Manager). Ambedue sono forniti con i relativi `test/queue_test.c` e `tes/smm_test.c`. È buona norma corredare ogni nuovo ADT col corrispettivo file `test/*_test.c`.

## Libreria

Sto scrivendo, in aggiunta a phase1 un leggero set di moduli e "di contorno", alcuni dei quali saranno solo d'aiuto per lo sviluppo di phase2, mentre altri verranno acclusi nella libreria finale per lo sviluppo di programmi per il sistema.

## Dettagli implementativi
	
Le code sono (da specifica AMIKaya) double-linked, circular, tail-pointed. La ricerca al loro interno è effettuata con linear-probe.

Il Gestore di Memoria Statica (SMM) istanzia un vettore di dimensioni fissate di tipo generico *T* ed uno stack di *"puntatore a T"* (sempre di dimensione costante) che contiene i puntatori agli elementi liberi nel vettore (dopo la chiamata di inizializzazione, lo stack è pieno e contiene tutti i puntatori). Ad ogni chiamata di `alloc()`, il puntatore affiorante nella pila viene estratto e ritornato (se possibile, altrimenti `NULL`), mentre a fronte di una chiamata di free(), l'elemento "deallocato" ritorna nello stack.

Il nucleo (sviluppato in phase2) contiene uno *scheduler* con politica *round robin*, oltre al supporto di base per il *message passing paradigm*. Le funzioni interne e le strutture dati del nucleo sono contenute nel file `src/kernel.c`. Sono tutte inaccessibili dall'esterno, tranne la funzione `SSIRequest()` che serve a richiedere servizi al *System Service Thread*.

	Autore: Andrea Simonetto
	Ultima modifica: 6 luglio 2007
	Adattato per Markdown: 27 luglio 2024
