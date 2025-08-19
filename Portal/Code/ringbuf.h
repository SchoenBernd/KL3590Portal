/* EasyCODE V8 */
/* EasyCODE ( 0 
ringbuf.h */
#define SDLAENGE 100
/* EasyCODE ( 0 
Definition der Klasse KlasseRBuf */
/* EasyCODE C */
/* EasyCODE < */
class KlasseRBuf
/* EasyCODE > */
{
        private:
        unsigned char laenge;
        unsigned char rest;
        unsigned char read_data;
        unsigned char write_data;
        unsigned char read_offset;
        unsigned char write_offset;
        unsigned char *datenptr;
        
        unsigned char leercheck (void);
        unsigned char ueberlaufcheck (void);
        unsigned char warnungscheck (void);
public:
        KlasseRBuf (unsigned char inilaenge, unsigned char inirest, unsigned char *ptr);
        void loeschen (void);
        unsigned char status (void);
        unsigned char readbyte (void);
        unsigned char writebyte (unsigned char data);
        /* EasyCODE ( 0 
        unsigned char data(void) */
        /* EasyCODE F */
        unsigned char data(void)
        {
            return read_data;
        }

        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseRBuf sdbuf;
/* EasyCODE ) */
