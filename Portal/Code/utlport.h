/* EasyCODE V8 */
/* EasyCODE ( 0 
utl3590.h */
// Datei:        utl3590.h
// Klasse:       Kl3590
// Ersteller:    Halfmann   EES 2856

// Prototypen
unsigned char ems_map (int handle, int logp, unsigned char physp);
int ems_alloc (int pages);
unsigned char ems_free (int handle);
int axtoi(char *arg);
void safdat_check (void);
void textload (void);
void com1_com3_init (void);

// globale Daten
extvol unsigned char textload_err;
/* EasyCODE ) */
