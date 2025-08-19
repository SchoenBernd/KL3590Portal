/* EasyCODE V8 */
/* EasyCODE ( 0 
konv3590.h */
// Dipl. Ing. Wilfried Halfmann

// Headerdatei zum Modul Konv3590

// Modul Konv3590  Dateiname: Konv3590.H

#define TRUE 1
#define FALSE 0
#define MaxRiegelstichzahl 9
/* EasyCODE ( 0 
typedefs */
/* EasyCODE < */
typedef unsigned char byte;
typedef unsigned int word;
typedef long unsigned int dword;
typedef unsigned short offs_adr;
typedef enum
{
  keine, k3568, k3568l, k3370, k3570, ksm200,     /* 5 */
  plotter, k3568_leiche, k118, monklasse, k3575,  /* 10 */
  k3576, k3568sp, k3568_2, k3574, k3568sp_2,      /* 15 */
  k3568l_2, symbol, zsk, fortron, tajima,         /* 20 */
  fmc, fdr3, barudan, zangs, pfaff,               /* 25 */
  hpgldaten, k3568_88, k3574_gr, k3588, k3587,    /* 30 */
  k3590, k3108, k3375, k3317, kportal             /* 35 */
} klassetyp;
typedef byte abschnarttype;   /* gerade..naehprogkenn */
typedef enum
{
  mittigli, mittigre, livongrundl, revongrundl
} stichrtyp;
/* EasyCODE > */
/* EasyCODE ( 0 
ikoordtyp */
/* EasyCODE C */
/* EasyCODE < */
typedef struct ikoordtyp
/* EasyCODE > */
{
        int x;
        int y;
}ikoordtyp;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
fkoordtyp */
/* EasyCODE C */
/* EasyCODE < */
typedef struct fkoordtyp
/* EasyCODE > */
{
        float x;
        float y;
}fkoordtyp;
/* EasyCODE E */
/* EasyCODE ) */
typedef fkoordtyp koordinate;

typedef unsigned char boolean;
/* EasyCODE ( 0 
anfriegelstruct */
/* EasyCODE C */
/* EasyCODE < */
typedef struct anfriegelstruct
/* EasyCODE > */
{
        int RStiche;
        int VStiche;
        int RTeilstiche;
        int VTeilstiche;
        koordinate Stich [MaxRiegelstichzahl+1];
        boolean fertig;
        int AktStichnr;
}anfriegeltype;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Definition der Klasse KlasseKonvert */
/* EasyCODE C */
/* EasyCODE < */
class KlasseKonvert
/* EasyCODE > */
{
        private:
        /* EasyCODE ( 0 
        Defines */
        /* EasyCODE < */
        /*----------------------------------------------------------------------------
        |   Funktionsmakros
        ----------------------------------------------------------------------------*/
        #define trunc(fval)  (int)floor(fval)
        #define round(fval)  ((int)floor(fabs(fval) + 0.5) * ((fval>0.0)? 1:-1))
        /* EasyCODE > */
        /* EasyCODE ( 0 
        fsqr */
        /* EasyCODE F */
        /*----------------------------------------------------------------------------
        |   Mathematische Funktionen
        ----------------------------------------------------------------------------*/
        float fsqr(float x)
        {
            return x*x;
        }

        /* EasyCODE ) */
        /* EasyCODE < */
        #define LPSTICHL 30.0          /* Stichlaenge im Ladepunktprogramm in 1/10 mm  */
        #define MAXINCR 126            /* Maximales Increment                          */
        #define MFKTPUFFERGR 10  /* Grî·e des Verschiebepuffers fÅr M-Funktionen */
        #define PI 3.141592654
        /* EasyCODE > */
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        typedefs */
        /* EasyCODE < */
        typedef float abbmatrixtyp[2][3];     /* 1..2; 1..3 */
        
        typedef fkoordtyp punktarraytyp [5];  /* -2..2 */
        
        typedef fkoordtyp parraytyp[4];       /* 1..4 */
        
        typedef float tarraytyp[5];           /* -2..2 */
        
        typedef enum
        {
          geteilt, einfach
        } klammertyp;
        /* EasyCODE > */
        /* EasyCODE C */
        /* EasyCODE < */
        typedef struct mfunkttyp
        /* EasyCODE > */
        {
                byte laenge;
                byte befehl;
                byte operand;
                int  verschieb;
        }mfunkttyp;
        /* EasyCODE E */
        /* EasyCODE < */
        typedef mfunkttyp mfktpuffertyp [MFKTPUFFERGR];
        
        typedef enum
        {
          out1, out2, out3, out4, niveauverst,
          lochabs, mitloch, klammauf, zuspann1, zuspann2,
          liklammer, reklammer, zickzack, keinausg
        } ausgangtyp;
        /* EasyCODE > */
        /* EasyCODE_STRUCTURED_COMMENT_START */
        /*
        EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
        typedef enum
        {
        nix,
        step, backstep, firstsec, lastsec,
        find,
        del_absch, ins_absch, zmove,
        nullpkt, magnif, reswindow, s3000_window, digitize,
        firstfktmenue, nextfktmenue,
        displacement, snapin, rasterdef, change,
        fktmenue, bildaufbau, stdstichlaend,
        koordbezp, startblk, endblk, blkmanip,
        magnblk, aendkurvenstatus,
        moveblk, rotateblk, mirrorblk,
        delblk, saveblk, makepara, blkumk, symbolcall,
        blkforw, blkrev, fileforw, filerev, parmoveblk,
        blkzukurve, rubberchange, frei_hand, parvorw, parrueckw,
        monogramm, rechtsb, linksb, schrifthoehe, seitenverh,
        buchstabst, monostichl, mittig, monoabbr, monoschneiden,
        kreisabb, blkzustich, blkwegopti, reserve44, zeigestiche,
        stichgen, zzmittli, zzmittre, zzli, zzre,
        
        endedit, esc,
        zsave,
        endefunc, endedigit, endezeichn,
        endblkmanip, endsymcall,
        endgrundl, endmono, endpara,
        
        gerade, kreisbogen,
        kreis, kurve, bgnstuetzpkt,
        krsstuetzpkt, kurvenende, stich, reserve60,
        reserve61, reserve62, reserve63, reserve64, reserve65,
        
        nameinfo, kommentarinfo,
        
        refpunktang,
        annaehen, schneiden,
        klammercode, reserve109,
        reserve110, stop,
        naehenaus, programmende,
        eilgang, langstich,
        reserve151, reserve152,
        outset, outres,
        wartenh, wartenl,
        nadel,
        warten,
        klammerform,
        standardstichl,
        bohrtiefe,
        zyklanf, zyklende,
        hindernis, lpprogende,
        
        voreinst,
        
        anfriegel, endriegel, reserve165, reserve166,
        
        liklammerauf, reklammerauf,
        zickzackein, zickzackaus, zuspannein, zuspannaus,
        drehzahlaend, stichlaengenaend, stichbreitenaend,
        reddrehzahlein, reddrehzahlaus,
        liklammerab, reklammerab,
        niveaufussein, niveaufussaus, reserve118,
        bnullpkt, mnullpkt, ladepkt, anfpkt, pattenanf,
        spannein, spannaus, folgeband, bohrein, bohraus,
        rahmein, rahmaus, reserve128, reserve129, reserve130,
        nadelstop, stopno, stopnu, stlochabsein, stlochabsaus,
        mitgehstlein, mitgehstlaus, klverrauf, klverrzu, bereichkenn,
        absdrehz, reserve142, reserve143, reserve144, reserve145,
        reserve146, reserve147, reserve148, reserve149, reserve150, 
        
        reserve200,  res177,
        
        res178, res179,
        penup, pendown, penselect, schrgroesse, schrwinkel,
        schrtext, linetype, liunteck, reobeck, res189,
        res190, res191, res192, res193, res194, res195, res196, res197, res198, res199,
        res200, res201, res202, res203, res204, res205, res206, res207, res208, res209,
        res210, res211, res212, res213, res214, res215, res216, res217, res218, res219,
        res220, res221, res222, res223, res224, res225, res226, res227, res228, res229,
        res230, res231, res232, res233, res234, res235, res236, res237, res238, res239,
        res240, res241, res242, res243, res244,
        verschoben, verschendpkt, gekillt, hilfskonstr, hilfskonst2,
        res250, res251, res252, res253, grunddatkenn, naehprogkenn
        } commandtyp;
        
        typedef byte mfkttype;            EasyCODE_COMMENT_START nameinfo..reserve200 EasyCODE_COMMENT_END
        EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
        */
        /* EasyCODE_STRUCTURED_COMMENT_END */
        /* EasyCODE < */
        typedef enum
        {
        frei00, frei01, frei02, frei03, frei04, frei05, frei06, frei07, frei08, frei09, frei0a, frei0b, frei0c, frei0d, frei0e, frei0f,   // 0x00 - 0x0f
        frei10, frei11, frei12, frei13, frei14, frei15, frei16, frei17, frei18, frei19, frei1a, frei1b, frei1c, frei1d, frei1e, frei1f,   // 0x10 - 0x1f
        frei20, frei21, frei22, frei23, frei24, frei25, frei26, frei27, frei28, frei29, frei2a, frei2b, frei2c, frei2d, frei2e, frei2f,   // 0x20 - 0x2f
        frei30, frei31, frei32, frei33, frei34, frei35, frei36, frei37, frei38, frei39, frei3a, frei3b, frei3c, frei3d, frei3e, frei3f,   // 0x30 - 0x3f
        frei40, frei41, frei42, frei43, frei44, frei45, frei46, frei47, frei48, frei49, frei4a, frei4b, frei4c, frei4d, gerade, kreisbogen,  // 0x40 - 0x4f
        kreis, kurve, bgnstuetzpkt, krsstuetzpkt, kurvenende, stich, frei56, frei57, frei58, frei59, frei5a, frei5b, nameinfo, frei5d, frei5e, annaehen,  // 0x50 - 0x5f
        schneiden,klammercode, frei62, ufwstop, stop, naehenaus, programmende, eilgang, frei68, frei69, frei6a, outset, outres, wartenh, wartenl, frei6f,  // 0x60 - 0x6f
        warten, klammerform, standardstichl, frei73, frei74, frei75, hindernis, lpprogende, frei78, anfriegel, endriegel, frei7b, frei7c, liklammerauf, reklammerauf, zickzackein,   // 0x70 - 0x7f
        zickzackaus, zuspannein, zuspannaus, frei83, stichlaengenaend, stichbreitenaend, reddrehzahlein, reddrehzahlaus, liklammerab, reklammerab, niveaufuss, hubfuss, frei8c, bnullpkt, mnullpkt, ladepkt,   // 0x80 - 0x8f
        anfpkt, pattenanf, spannein, spannaus, fussniveaulevel, fusshublevel, frei96, frei97, frei98, frei99, frei9a, frei9b, frei9c, frei9d, frei9e, stlochabsein,   // 0x90 -0x9f
        stlochabsaus, mitgehstlein, mitgehstlaus, klverrauf, klverrzu, freia5, absdrehz, freia7, freia8, freia9, freiaa, freiab, freiac, freiad, freiae, freiaf,  // 0xa0 - 0xaf
        setflag, resflag, freib2, freib3, freib4, freib5, freib6, freib7, freib8, freib9, freiba, freibb, freibc, freibd, freibe, freibf, // 0xb0 - 0xbf
        freic0, freic1, freic2, freic3, freic4, freic5, freic6, freic7, freic8, freic9, freica, freicb, freicc, freicd, freice, freicf,   // 0xc0 - 0xcf
        freid0, freid1, freid2, freid3, freid4, freid5, freid6, freid7, freid8, freid9, freida, freidb, freidc, freidd, freide, freidf,   // 0xd0 - 0xdf
        freie0, freie1, freie2, freie3, freie4, freie5, freie6, freie7, freie8, freie9, freiea, freieb, freiec, freied, freiee, freief,   // 0xe0 - 0xef
        freif0, freif1, freif2, freif3, freif4, freif5, freif6, freif7, freif8, freif9, freifa, freifb, freifc, freifd, freife, naehprogkenn,  // 0xf0 - 0xff
        } commandtyp;
        
        typedef byte mfkttype;            /* nameinfo..reserve200 */
        /* EasyCODE > */
        /* EasyCODE C */
        /* EasyCODE < */
        typedef struct abschntype
        /* EasyCODE > */
        {
                abschnarttype   abschnittart;
                /* EasyCODE ( 0 
                Parameter */
                /* EasyCODE C */
                /* EasyCODE < */
                union
                /* EasyCODE > */
                {
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                boolean vorhanden;
                        }shindernis;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                int stichlaenge;
                        }sstichlaengenaend;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                int stichbreite;
                                stichrtyp stichr;
                        }sstichbreitenaend;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                int name;
                        }snameinfo;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                int code;
                        }sklammercode;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                ikoordtyp koord;
                        }skoordabsch;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                int del;
                                int sonst;
                        }sabsdrehz;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                dword memdwort;
                        }shilfskonstr;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                word firstword;
                                word secword;
                        }shilfskonst2;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                klassetyp klasse;
                                word versnr;
                        }snaehprogkenn;
                        /* EasyCODE E */
                        /* EasyCODE C */
                        /* EasyCODE < */
                        struct
                        /* EasyCODE > */
                        {
                                char VorwStiche;
                                char VorwTeilstiche;
                                char RueckwStiche;
                                char RueckwTeilstiche;
                        }sriegel;
                        /* EasyCODE E */
                }_VAR;
                /* EasyCODE E */
                /* EasyCODE ) */
        }abschntype;
        /* EasyCODE E */
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        interne Variablen und Funktionen */
        /* EasyCODE < */
        byte      fehler;
        boolean   progmitpatten, maschmitlpprogramm;
        
        int mindrehzahl,
            maxdrehzahl,
            maxstichl,     /* Maximale Stichlaenge in 1/10 mm */
            minnaehfeld_x,
            maxnaehfeld_x, /* Maximale X-Koordinate           */
            minnaehfeld_y,
            maxnaehfeld_y; /* Maximale Y-Koordinate           */
        
        /* Offsets verschiedener Elemente des Abschnittdatensatzes */
        offs_adr geooffs,          /* Begin des Geometrischen Datensatzes */
                 kennoffs,             /* Maschinenkennung */
                 klammeroffs,       /* KlammerArt */
                 hindernoffs,         /* Hindernisse */
                 mnullpoffs,          /* Maschinennullpunkt */
                 abschdataoffs;    /* Abschnittdatensatz */
        
        /* Offsets verschiedener Elemente des Naehdatensatzes */
        offs_adr naehdatoffs;   /* Naehprogramms */
        
        mfktpuffertyp mfktpuffer;  /* PufferArray fuer M-Funktionen mit positivem Del */
        
        fkoordtyp stichnachlinks,  /* Vektor senkrecht zur Nahtrichtung */
                  letztereinstich, /* Absolutkoordinate des zuletzt ausgegebenen Einstichpunktes */
                  letztereinstich_mem, /* Merker fÅr letzter Einstich */
                  letztekoord;
        
        ikoordtyp eilinc, bildanfkoord, lpkoord, stapelkoord, pattenanfkoord;
        
        float stichbr, stichlausdatens, fstichl;
        
        int i_stdstichl;
        
        offs_adr abschanf,    /* Anfangsadresse des Abschnittdatenstzes    */
                 maschanf,    /* Anfangsadresse des Maschinenprogramms     */
                 lpmaschadr,  /* absolute Adresse der Funktion LpProgEnde  */
                 firstc,
                 currentc,
                 lastc,       /* Zur Kurvenberechnung */
                 maschadr,    /* Aktuelle Adresse im Maschinendatensatz */
                 abschadr,    /* Aktuelle Adresse im geometrischen Datensatz */
                 maxmem;
        
        word anzahlstiche;
        
        boolean fehler14, stini, lpprogramm, mfktinpuffer, riegelanfang, riegelende, fadengeschnitten, maschinenaeht;
        boolean stichgemacht;  /* zeigt an, ob nach dem Annaeh-Befehl ein Stich gemacht wurde */
        boolean stichrichtungli, ausgang_gesetzt[ 8 ], ausg_niveau, ausg_lochabs, ausg_liklammer, ausg_reklammer;
        boolean ausg_zuspann, ausg_zickzack;
        boolean stichgemacht_mem;
        
        stichrtyp stichlage;
        
        koordinate EndRiegelKoord [MaxRiegelstichzahl+1];
        koordinate RiegelsticheV [37], RiegelsticheR [37];
        
        /*----------------------------------------------------------------------------
        | Prototypen der intern verwendeten Unterprogramme
        ----------------------------------------------------------------------------*/
        
        /* Hilfsunterprogramme zur Inbetriebnahme */
        /* EasyCODE > */
        #if defined debug_info
                /* EasyCODE < */
                void abschn_ausgeben (abschntype abschnitt);
                void i_koord_ausgeben (ikoordtyp koord);
                void f_koord_ausgeben (fkoordtyp koord);
                /* EasyCODE > */
        #endif 
        /* EasyCODE < */
        /* Initialisierung */
        void init_masch_param (void);
        
        /* Speicherverwaltung */
        void wrabsch (abschntype * abschnitt, offs_adr offset);
        void rdabsch (abschntype * abschnitt, offs_adr offset);
        void wrdword (offs_adr offset, dword datenwort);
        void wrword  (offs_adr offset, word datenwort);
        void wrbyte  (offs_adr offset, byte maschbyte);
        byte membyte (offs_adr offset);
        void movblock (offs_adr blockanf, offs_adr blockende, word anzahl);
        void resetmem (void);
        offs_adr vorg (offs_adr offset);
        offs_adr nachf (offs_adr offset);
        void readnextabsch (abschntype * abschnitt);
        
        
        /* Mathematische Unterprogramme */
        
        float drehwinkel (koordinate mittelpunkt, koordinate endpunkt);
        void kreisaus3punkten (koordinate punkt1,
                                              koordinate punkt2,
                                              koordinate punkt3,
                                              float * radius,
                                              koordinate * mittelpunkt,
                                              float * richtung,
                                              byte * fehler);
        
        float abschnittwinkel (koordinate anfpunkt,
                                           koordinate endpunkt,
                                           float richtung,
                                           koordinate mittelpunkt,
                                           byte * fehler);
        
        void berechnevektor (float radius,
                                           float stichwinkel,
                                           koordinate * vektor,
                                           float betrstichnlinks,
                                           boolean * stichrichtungli,
                                           stichrtyp stichlage);
        
        //void initmatrix (float **abbmatrix);
        
        //void matmultipl (float **matrix1,
        //                 float **matrix2);
        
        
        void initmatrix (abbmatrixtyp abbmatrix);
        
        void matmultipl (abbmatrixtyp matrix1,
                         abbmatrixtyp matrix2);
        
        /* Hilfsunterprogramme zur Datenausgabe */
        
        byte vorzbetr (int intzahl);
        void writefkt (mfunkttyp maschfkt, offs_adr adresse);
        void pufferschreiben (mfunkttyp maschfkt);
        void mfktausgeben (mfunkttyp maschfkt);
        void stichpunktausgeben (fkoordtyp koord);
        
        /* Unterprogramme zur Bearbeitung der einzelnen Abschnittarten */
        boolean is_koordabschn (abschnarttype abschnittart);
        boolean is_kurvenpunkt (abschnarttype abschnittart);
        void berechneueberstich (fkoordtyp einstichpunkt,
                                                  fkoordtyp * ueberstich,
                                                  fkoordtyp stichnachlinks,
                                                  boolean   * stichrichtungli,
                                                  stichrtyp stichlage);
        
        void bearbeitefunktion (abschntype abschnitt);
        void bearbeitestich (abschntype abschnitt);
        void bearbeitegerade (abschntype abschnitt);
        void bearbeitekreis (abschntype abschnitt, boolean abschnittsweise);
        
        /* Unterprogramme zur Kurvenberechnung */
        
        void berechnekurvenlaenge (offs_adr  puffer, float     *laenge);
        void berechne_t (offs_adr  puffer, tarraytyp t);
        void suchekurvenpunkte (offs_adr       puffer,
                                                 punktarraytyp  pktarray,
                                                 boolean        * kurvenanfpkt,
                                                 boolean        * kurvenendpkt,
                                                 tarraytyp      t);
        void berech_polynom (float     t,
                                             parraytyp p,
                                             float     arg,
                                             fkoordtyp * polypkt);
        void berechnenormalenvektor (parraytyp p,
                                                          tarraytyp t,
                                                          float     aktt,
                                                          float     betrstichnlinks,
                                                          fkoordtyp *stichnachlinks);
        void cf (punktarraytyp punkt,
                     tarraytyp     t,
                     boolean       kurvenanfang,
                     boolean       kurvenende,
                     parraytyp     p,
                     byte          *fehler);
        void bearbeitekurve (boolean abschnittsweise);
        void stichout (fkoordtyp zielkoord, fkoordtyp *istkoord);
        /* EasyCODE > */
        /* EasyCODE ) */
public:
        /* EasyCODE ( 0 
        Daten */
        /* Zeiger auf das erste Byte des NÑhprogramms (fÅr Speicherverwaltung) */
        byte huge * prog_anf;
        
        anfriegeltype AnfRiegelStatus;
        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Konstruktor */
        /* EasyCODE F */
        KlasseKonvert (void)
        {
        }

        /* EasyCODE ) */
        /* EasyCODE ( 0 
        Funktionen */
        /* EasyCODE < */
        void suchekoord (offs_adr abschadr, ikoordtyp * punkt);
        
        void verschiebe_abschnitt (offs_adr puffer, ikoordtyp punkt);
        
        void block_abb (offs_adr blockanf,
                        offs_adr blockende,
                        ikoordtyp nullpunkt,
                        float scalex,
                        float scaley,
                        float drehwinkel,
                        byte spiegeln);
        
        byte absch_gen (byte huge * proganf,  // Zeiger auf das erste Byte des Programms
                        word std_stichl,      // StandardstichlÑnge
                        word mem_avail,       // Grî·e des Speicherbereichs
                        offs_adr abschadr,    // Adresse des zu konvertierenden Abschnitts
                        offs_adr * maschadr); // Adresse der Stichdaten
        
        byte konvert (byte huge * proganf,    // Zeiger auf das erste Byte des Programms
                      word std_stichl,        // StandardstichlÑnge
                      word mem_avail,         /* Grî·e des Speicherbereichs              */
                      word * abschnr);        // Nummer des fehlerhaften Abschnitts
                                              // wenn konvert != 0
        /* EasyCODE > */
        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
extern KlasseKonvert konv;
/* EasyCODE ) */
