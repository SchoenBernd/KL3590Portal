/* EasyCODE V8 */
/* EasyCODE ( 0 
sdups.h,   Ersteller: Wilfried Halfmann   EES 2856 */
/* EasyCODE ( 0 
Defines */
/* EasyCODE ( 0 
Kommandos */
#define         F_INIT_MEDIA        "\x01"    // uchar (void)
#define         F_FQUICK_FORMAT     "\x02"    // int (void) 
#define         F_GET_VOL_ID        "\x03"    // void(void)-> serial
#define         F_READ_DIR          "\x04"    // void(void)-> serial
#define         F_GET_FILE_INFO     "\x05"    // int (uchar *NAME, ulong *F_SIZE, uchar *F_CREATE, uchar *F_MODIFY,
#define         F_CH_DIR            "\x06"    // 
#define         F_CARD_IN_SLOT      "\x07"
#define         F_OPEN              "\x08"    // FILE* (uchar* name, uchar MODE)
#define         F_MK_DIR            "\x09"    // int (char* F_PATH)
#define         F_CREATE            "\x0a"    // FILE* (uchar* name,uchar MODE)
#define         F_REM_DIR           "\x0b"    // int (char* F_PATH)
#define         F_REMOVE            "\x0c"    // int (uchar* name)
#define         F_RENAME            "\x0d"    // int (uchar* NAME_OLD,uchar* NAME_NEW)
#define         F_CLOSE             "\x0e"    // int (FILE* rp)
#define         F_FLUSH             "\x0f"    // int (FILE* rp)
#define         F_FREEMEM           "\x10"    // int (FILE* rp)
#define         F_TELL              "\x11"    // long (FILE* rp)
#define         F_SEEK              "\x12"    // int (FILE* rp,ulong offset,uchar mode)
#define         F_GETC              "\x13"    // int (FILE* rp)
#define         F_READ              "\x14"    // int (void* rd_buff,uint dat_size,uint num_item,FILE* rp)
#define         F_GETS              "\x15"    // 
#define         F_UNGET             "\x16"    // 
#define         F_PUTC              "\x17"        
#define         F_WRITE             "\x18"
#define         F_PUTS              "\x19"        
#define         F_EOF               "\x1a"
#define	      F_SDMODE	  "\x1b"
#define	      F_NETMODE	  "\x1c"

#define         SD_ESC              0x81
#define         BDF_ESC             0xFF    // noch nicht festgelegt
#define         OPCODEA             0x0E    // next code is a CHR code
#define         OPCODEB             0x0F    
#define         FILE_NOT_AVAILABLE  30      // als Rueckmeldung wenn keine freie file handle zur Verfuegung ist.
/* EasyCODE - */
#define EINLEITUNG_P1 "\x1bM"
#define HANDLES "\x01"
#define HANDLE 0x01
#define FILEATTRIBUT "\x80"     // Datei normal
/* EasyCODE - */
// fopen mode options
#define         M_READ        "\x81"
#define         M_WRITE       "\x82"
#define         M_APPEND      "\x84"
/* EasyCODE - */
// seek mode options
#define         M_SEEK_CUR    "\x80"
#define         M_SEEK_END    "\x81"
#define         M_SEEK_SET    "\x82"
/* EasyCODE - */
// Bdf-P1
#define INITMEDIA_P1 EINLEITUNG_P1 F_INIT_MEDIA
#define GETLABEL_P1 EINLEITUNG_P1 F_GET_VOL_ID
#define OPEN_P1 EINLEITUNG_P1 F_OPEN M_READ
#define OPENWRITE_P1 EINLEITUNG_P1 F_OPEN M_WRITE
#define CLOSE_P1 EINLEITUNG_P1 F_CLOSE HANDLES
#define CREATE_P1 EINLEITUNG_P1 F_CREATE FILEATTRIBUT
#define CHANGEDIRECTORY_P1 EINLEITUNG_P1 F_CH_DIR
#define KARTENSTATUS_P1 EINLEITUNG_P1 F_CARD_IN_SLOT
#define WRITE_P1 EINLEITUNG_P1 F_WRITE HANDLES
#define SEEK_P1 EINLEITUNG_P1 F_SEEK HANDLES
#define TELL_P1 EINLEITUNG_P1 F_TELL HANDLES
#define READ_P1 EINLEITUNG_P1 F_READ HANDLES
#define DELETE_P1 EINLEITUNG_P1 F_REMOVE
#define FORMATIEREN_P1 EINLEITUNG_P1 F_FQUICK_FORMAT
#define MAKEDIRECTORY_P1 EINLEITUNG_P1 F_MK_DIR
#define READDIRECTORY_P1 EINLEITUNG_P1 F_READ_DIR
#define MODESD_P1 EINLEITUNG_P1 F_SDMODE
#define MODENET_P1 EINLEITUNG_P1 F_NETMODE
/* EasyCODE ) */
/* EasyCODE ( 0 
Rueckmeldungen */
#define SD_INIT_OK             0
#define SD_INIT_FAILED         1
#define SD_INIT_FAIL2          2
#define FILE_OPEN_FAILED       3
#define FILE_IS_OPENED         4
#define FILE_CREATION_FAILED   5
#define FILE_IS_CREATED        6
#define FILE_CLOSED            7
#define FILE_CLOSE_FAILED      8
#define FILE_REF_NOT_VALID     9
#define FILE_REF_OK            10
#define GETCHAR_FAILED         11
#define WRITE_ERROR            12
#define FAIL_TO_SEEK           13
#define SEEK_OK                14
#define FTELL_OK               15
#define FTELL_FAILED           16
#define FEOF_FAILED            17
#define DPTR_IS_NOT_AT_END     18
#define DPTR_IS_AT_END         19
#define FILE_REMOVED           20
#define FILE_NOT_REMOVED       21
#define FILE_RENAME_FAILED     22
#define FILE_RENAME_OK         23
#define FORMAT_FAILED          24
#define FORMAT_COMPLETE        25
#define CHANGE_DIR_FAIL        26
#define CHANGE_DIR_OK          27
#define MAKE_DIR_FAIL          28
#define MAKE_DIR_OK            29
#define MODE_SD_FAIL	  	  34
#define MODE_SD_OK		  35
#define MODE_NET_FAIL        36
#define MODE_NET_OK		  37

#define CARD_IS_WR_PROTECT      0x03
#define CARD_INSERETION_FAIL    0x04
#define NO_CARD_INSERTED        0x05
#define CARD_IS_WR_ENABLE       0x02
/* EasyCODE ) */
// Parameter beim File oeffnen
#define READ   0
#define WRITE  1

// Zeiten
#define T1SEK 100    // 1s
#define T3SEK 300    // 3s
#define T10SEK 1000  // 10s
#define T30SEK 3000  // 30s
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse KlasseSdUp */
/* EasyCODE C */
/* EasyCODE < */
class KlasseSdUp
/* EasyCODE > */
{
        private:
public:
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseSdUp(void)
        {
        }

        /* EasyCODE ) */
        int SdInitMedia (void);
        int SdCardStatus (void);
        int SdChangeDir (char *dir);
        int SdReadDir (char *directory, char *dirstring);
        int SdMakeDir (char *name);
        int SdFileOpen (char *dateiname, unsigned char write);
        int SdFileClose (void);
        int SdFileCreate (char *dateiname);
        int SdFileWrite (char *programmdaten, unsigned int anzahl);
        int SdFileRead (char *programmdaten, unsigned long laenge);
        int SdFileDelete (char *dateiname);
        int SdFormatCard (void);
        int GetFileSize (char *dateiname, unsigned long *filesize);
        int ChangeToSD (void);
        int ChangeToNET (void);
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseSdUp sd;
/* EasyCODE ) */
