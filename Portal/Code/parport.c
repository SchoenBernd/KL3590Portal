/* EasyCODE V8 */
/* EasyCODE ( 0 
parport */
/* EasyCODE ( 0 
Beschreibung */
/* Datei: parport.C       Klasse: 3590 Portal      Ersteller: Saffarian TEE 3461

Nach der Deklaration vom Parameter-Object soll die initialisiert werden.

Zum Beispiel:

Klasse Par myPar;
myPar.Init(zeiger auf Parameter-struktur-Array , anzahl der Parameter);

// ab dann koenen Sie folgender Funktionen aufrufen um die Parameter zuzugreifen.
// oder par.grundzustand() um alles ueber Bedienfeld zu bearbeiten.

**********************************************************************
void* GetParPtr(unsigned int number,unsigned char n=0)
**********************************************************************
  return a void pointer to parameter (not struct pointer!)
  number: parameter number
  n: array index (only if parameter is an array) default = 0

**********************************************************************
unsigned char grundzustand(void)
**********************************************************************
 Um die Parameter ueber Bedienfeld zu aendern,
 sollte diese Funktion aufgerufen werden.

**********************************************************************
char GetParType(unsigned int number);
**********************************************************************
 Es meldet Parametertyp zurueck, ob char oder int oder (siehe header)
 number: parameter number

**********************************************************************
int GetParIndex(unsigned int number)
**********************************************************************
 Es meldet index von Parameter zurueck. Z.B. wenn parameter 704 die 5. Parameter
 in Struktur-array ist, diese Funktion meldet 4 zurueck.

**********************************************************************
int GetSize()
**********************************************************************
 Rueckmeldung ist die Anzahl der Parameter in Object.

**********************************************************************
char SetFreigabe(char level)
**********************************************************************
 Diese Funktion stellt die Freigabestufe von Object fest.
 level: ACC0..ACC4 (Siehe Header)

**********************************************************************
char GetFreigabe(void)
**********************************************************************
 Diese Funktion meldet die Freigabestufe von Object zurueck.ACC0..ACC4 (Siehe Header)

**********************************************************************
int GetArraySize(unsigned int number)
**********************************************************************
 Es meldet die Grosse von Array Parameter zurueck.
 number: parameter number

**********************************************************************
void SetPar(unsigned int number,void* ptr,unsigned char n = 0)
**********************************************************************
 Setzt die Parameterwert ein.
 number:parameter number
 ptr:   Zeiger auf neue Wert
 n:     index (falls Parameter ein Array ist)(default = 0)

**********************************************************************
void IncPar(unsigned int number,unsigned char n=0,double grew=1)
**********************************************************************
 Parameter wird inkrementiert.
 number:parameter number
 n:     index (falls Parameter ein Array ist)
 grew:  Inkrement-Wert (default = 1)

**********************************************************************
void DecPar(unsigned int number,unsigned char n=0,double grew=1)
**********************************************************************
 Parameter wird dekrementiert.
 number:parameter number
 n:     index (falls Parameter ein Array ist)
 grew:  Inkrement-Wert (default = 1)
**********************************************************************
char KaltStart(void)
**********************************************************************
 Alle Parameter werden von Defaultwert geladen

**********************************************************************
char CheckUp(char corr)
**********************************************************************
 Diese Methode ueberprueft, ob alle Parameter in gultigem Bereich sind
 corr: if==TRUE then die nicht gultige Parameter werden mit Defaultwert geladen
 Rueckmeldung: Non zero wenn mindestens ein Parameter auserhalb des Bereich war.

**********************************************************************
void LoadDefault(st_parameter* ptr)
**********************************************************************
 Defaultwert wird geladen.
 ptr: die Adresse von Parameter Struktur.

**********************************************************************
void Init(st_parameter* ptr,int s)
**********************************************************************
 Parameter Object wird initialisiert
 ptr: Zeiger auf Parameterstruktur-Array
 s  : Anzahl der existierte Parameter

**********************************************************************
char ShowName(char endis)
**********************************************************************
 Es gibt die Möglichkeit um einen Hilftext fuer jede Parameter darzustellen.
 Rufen Sie diese Fuktion mit endis=TRUE damit diese Eigenschaft eingeschaltet wird.

**********************************************************************
double GetDefault(int number,int n)
**********************************************************************
 Es meldet den Defaultwert von Parameter zurueck.
 number:parameter number
 n:     index (falls Parameter ein Array ist)

**********************************************************************
double GetDefault(st_parameter* ptr,int n=0)
**********************************************************************
 ptr is the pointer to parameter structure
 n is the index if parameter is an array. default = 0;
 return the actual default value. Wenn DEFDEP flag eingesetzt ist, wird der
 richtige Wert berechnet.
 Siehe  GetParDef()

**********************************************************************
char GetParVal(unsigned int number,double* dest,unsigned char n=0)
**********************************************************************
 parameter value will be loaded in dest without factor multiplication.
 number is the parameter number
 n is the index of parameter array if parameter is an array. Default value is zero.
 return TRUE if successful.

**********************************************************************
char GetParDef(unsigned int number,double* dest,unsigned char n=0)
**********************************************************************
 default value from table will be loaded in dest without any change.
 number is the parameter number
 n is the index of parameter array if parameter is an array. Default value is zero.
 return TRUE if successful.
 Siehe GetDefault()

**********************************************************************
char GetParDef(st_parameter* ptr,double* dest,unsigned char n=0)
**********************************************************************
 default value from table will be loaded in 'dest' without any change.
 ptr is the parameter structure address
 n is the index of parameter array if parameter is an array. Default value is zero.
 return TRUE if successful.

**********************************************************************
char Norm()
**********************************************************************
 checks all parameter values.
 if greather than max then = max
 if less than min then = min
 and return value is none zero if at least one parameter
 has been out of range.

**********************************************************************
char GetUnitStr(unsigned int number,char* pStr)
**********************************************************************
 fills the pStr with the parameter unit string.This location must be enough long !
 number: is the parameter number
 return: number of characters in Null terminated string (inc. Null).

**********************************************************************
char IsChanged(unsigned int number); // Ob ein Parameter geaendert ist
**********************************************************************
 call this methode after grundzustand() to declear whether any parameter is changed.
 input: number is the parameter number.
 return value: TRUE if changed.
**********************************************************************
*/
/* EasyCODE ) */
#include "portal.h"
/* EasyCODE ( 0 
Daten */
/* EasyCODE ( 0 
default values */
const char pd101[1]={1};
const int  pd102[1]={1};
const char pd103[1]={5};
const char pd104[1]={5};
const char pd105[1]={1};
const char pd106[1]={15};
const char pd107[1]={0};
const char pd108[1]={1};
const char pd109[1]={0};
const char pd110[1]={0};
const int  pd111[1]={1};
const char pd114[1]={1};
const int  pd115[1]={XNAEHFELD_MIN};
const int  pd116[1]={XNAEHFELD_MAX};
const int  pd117[1]={YNAEHFELD_MIN};
const int  pd118[1]={YNAEHFELD_MAX};
const int  pd131[1]={DUENN};
const char pd140[1]={1};
const char pd141[1]={1};   // SW014
const char pd142[1]={1};
const char pd143[1]={0};
const char pd144[1]={1};
const char pd145[1]={0};
const unsigned int pd150[9]={20,20,20,20,20,20,20,20,20};
const unsigned int pd151[9]={20,20,20,20,20,20,20,20,20};
const char pd152[1]={0};
const char pd153[1]={0};
const char pd154[1]={0};


const char pd201[1]={0};  // SW005DIS
const char pd202[1]={0};
const char pd203[1]={0};
const char pd206[1]={10};
const char pd207[1]={10};
const char pd208[1]={0};

const int  pd301[1]={90};
const int  pd302[1]={75};
const int  pd303[1]={195};
const int  pd304[1]={288};
const int  pd305[1]={14};
const int  pd306[1]={180};

const int  pd401[1]={20};
const int  pd402[1]={30};
const int  pd403[1]={200};
const int  pd404[1]={200};
const int  pd405[1]={20};
const int  pd406[1]={30};
const char pd410[1]={2};
const char pd411[1]={2};

const char pd501[1]={30};
const char pd502[1]={18};
const char pd503[1]={4};
const char pd504[1]={3};

const char pd701[1]={12};
const char pd702[1]={30};
const char pd703[1]={100};
const char pd704[1]={1};
const char pd705[1]={40};
const char pd706[1]={25};
const char pd707[1]={0};
const char pd708[1]={18};
const int  pd709[1]={15};
const int  pd710[1]={15};
const char pd711[1]={0};
const char pd712[1]={15};
const char pd713[1]={1};
const char pd714[1]={22};
const char pd715[1]={22};
const char pd716[1]={2};
const char pd717[1]={1};

const char pd901[1]={0};
const double pd902[1]={0};
const unsigned long pd903[1]={0};
const char pd904[1]={1};
const char pd905[1]={2};
const char pd906[1]={46};
/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Costructors and Init */
/* EasyCODE ( 0 
Constructor
KlassePar(void) */
/* EasyCODE F */
KlassePar::KlassePar(void)
{
    // Constructor
    pSt = 0;
    size = 0;
    key = 1;
    group = 1;
    index = 1;
    arr = 0;
    lastKey = 0xFF;
    freigabe = 1;
    prtName = 0;
    pCompObj = 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Destructor
KlassePar(void) */
/* EasyCODE F */
KlassePar::~KlassePar(void)
{
    if ( pSt )
    {
        /* EasyCODE_STRUCTURED_COMMENT_START */
        /*
        EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
        double dest,src;
        EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
        if ( pCompObj )
        {
            EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
            // Each parameter will be compared with corresponding
            // parameter in reference object. If different, then
            // Invalid flag in reference object will be TRUE.
            EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
            for ( int i = 0 ; i < size ; i++ )
            {
                for ( int j = 0 ; j < pSt[i].size ; j++ )
                {
                    if ( GetPar(pSt[i].pVar,pSt[i].typ,&src,j) &&
                         pCompObj->GetPar(pCompObj->pSt[i].pVar,pSt[i].typ,&dest,j) )
                    {
                        if ( src == dest )
                        {
                        }
                        else
                        {
                            EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
                            pCompObj->pSt[i].Invalid = TRUE;
                            EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
                        }
                    }
                }
            }
        }
        else
        {
            EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
            // no reference object exist
            EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
        }
        */
        /* EasyCODE_STRUCTURED_COMMENT_END */
        // Free reserved memory for this object (parameter array)
        delete [] pSt;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Explicit constructor
KlassePar(st_parameter* ptr,int s) */
/* EasyCODE F */
KlassePar::KlassePar(st_parameter* ptr,int s)
{
    // explicit Constructor
    // ptr: pointer to parameter structure array
    size = s ; // Number of parameter in array
    key = 1;
    group = 1;
    index = 1;
    arr = 0;
    lastKey = 0xFF;
    freigabe = 1;
    prtName = 0;
    pCompObj = 0;
    /* EasyCODE - */
    pSt = new st_parameter[size];
    for ( int i = 0; i<size ;i++ )
    {
        pSt[i] = ptr[i];
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Init */
/* EasyCODE F */
void KlassePar::Init(st_parameter* ptr,int s)
{
    // ptr: pointer to parameter structure array
    size = s ; // Number of parameter in array
    key = 1;
    group = 1;
    index = 1;
    arr = 0;
    lastKey = 0xFF;
    freigabe = 1;
    prtName = 0;
    pCompObj = 0;
    /* EasyCODE - */
    pSt = new st_parameter[size];
    for ( int i = 0; i<size ;i++ )
    {
        pSt[i] = ptr[i];
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Copy constructor */
/* EasyCODE F */
KlassePar::KlassePar(const KlassePar& par)
{
    key = par.key;
    index = par.index;
    group = par.group;
    arr = par.arr;
    freigabe = par.freigabe;
    //pSt = par.pSt;
    size = par.size;
    lastKey = par.lastKey;
    prtName = par.prtName;
    pCompObj = par.pCompObj;
    /* EasyCODE - */
    pSt = new st_parameter[size];
    for ( int i = 0; i<size ;i++ )
    {
        pSt[i] = par.pSt[i];
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Copy constructor */
/* EasyCODE F */
KlassePar::KlassePar(const KlassePar* par)
{
    key = par->key;
    index = par->index;
    group = par->group;
    arr = par->arr;
    freigabe = par->freigabe;
    //pSt = par->pSt;
    size = par->size;
    lastKey = par->lastKey;
    prtName = par->prtName;
    pCompObj = par->pCompObj;
    /* EasyCODE - */
    pSt = new st_parameter[size];
    for ( int i = 0; i<size ;i++ )
    {
        pSt[i] = par->pSt[i];
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
operator = */
/* EasyCODE F */
const KlassePar& KlassePar::operator =(const KlassePar& par)
{
    key = par.key;
    index = par.index;
    group = par.group;
    arr = par.arr;
    freigabe = par.freigabe;
    //pSt = par.pSt;
    size = par.size;
    lastKey = par.lastKey;
    prtName = par.prtName;
    pCompObj = par.pCompObj;
    pSt = new st_parameter[size];
    for ( int i = 0; i<size ;i++ )
    {
        pSt[i] = par.pSt[i];
    }
    return *this;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
parameterinit(void) */
/* EasyCODE F */
void KlassePar::parameterinit(void)
{
    if ( parameterinit_enable )
    {
        /* EasyCODE ( 0 
        Definition der parameter */
        /* EasyCODE < */
        st_parameter parameter[]={
        // Name                            factor  group   number  min     max     size  default  type       access  depend  pointer               unit              (char)Invalid
        { "Unterfadenwaechter",            1,      1,      1,      0,      5,      1,    &pd101,  TYPCH,     ACC1,   NODEP,  &saf->md.ufwen,       ""}, //SW18-1
        { "Oberfadenwaechter",             1,      1,      2,      0,      1,      1,    &pd102,  TYPONOFF,  ACC1,   NODEP,  &saf->md.ofwen,       ""},
        { "Ausblendstiche OFW",            1,      1,      3,      0,     99,      1,    &pd103,  TYPCH,     ACC3,   NODEP,  &saf->md.ofwpreset,   "St."},
        { "Ausblendstiche UFW",            1,      1,      4,      0,     99,      1,    &pd104,  TYPCH,     ACC3,   NODEP,  &saf->md.ufwpreset,   "St."},
        { "Reaktionszeit OFW" ,            1,      1,      5,      1,      9,      1,    &pd105,  TYPCH,     ACC3,   NODEP,  &saf->md.fw_counter,  ""},
        { "Stichrueckfuehrung",            1,      1,      6,      0,     99,      1,    &pd106,  TYPCH,     ACC1,   NODEP,  &saf->md.backst,      "St."},
        { "Autom. Klammeroeffnen",         1,      1,      7,      0,      1,      1,    &pd107,  TYPONOFF,  ACC1,   NODEP,  &saf->md.openclampen, ""},
        { "Flip-Flop-Modus Fussschalter",  1,      1,      8,      0,      1,      1,    &pd108,  TYPONOFF,  ACC1,   NODEP,  &saf->md.ffmode,      ""},
        { "Klammerueberwachung",           1,      1,      9,      0,      2,      1,    &pd109,  TYPUCH,    ACC1,   NODEP,  &saf->md.jigctren,    ""},
        { "Demomodus",                     1,      1,     10,      0,      1,      1,    &pd110,  TYPONOFF,  ACC1,   NODEP,  &nae.demomodus,       ""},
        { "Modus Unterfadensch./Nadeldr.", 1,      1,     11,      0,      3,      1,    &pd111,  TYPUCH,    ACC1,   NODEP,  &saf->md.drehmodus,   ""},
        { "autom. Sequenzweiterschaltung", 1,      1,     14,      0,      1,      1,    &pd114,  TYPONOFF,  ACC1,   NODEP,  &saf->md.autoseq,     ""},
        { "min. X-Koordinate",             1,      1,     15, XNAEHFELD_MIN, 116,  1,    &pd115,  TYPINT,    ACC1,   MAXDEP, &saf->xnaehfeld_min,  "0,1mm"},
        { "max. X-Koordinate",             1,      1,     16,    115,XNAEHFELD_MAX,1,    &pd116,  TYPINT,    ACC1,   MINDEP, &saf->xnaehfeld_max,  "0,1mm"},
        { "min. Y-Koordinate",             1,      1,     17, YNAEHFELD_MIN, 118,  1,    &pd117,  TYPINT,    ACC1,   MAXDEP, &saf->ynaehfeld_min,  "0,1mm"},
        { "max. Y-Koordinate",             1,      1,     18,    117,YNAEHFELD_MAX,1,    &pd118,  TYPINT,    ACC1,   MINDEP, &saf->ynaehfeld_max,  "0,1mm"},
        { "Materialdicke/Gewicht",         1,      1,     31,      0,      1,      1,    &pd131,  TYPUCH,    ACC1,   NODEP,  &saf->md.matdicke,    ""},
        { "Fadenwedler",                   1,      1,     40,      0,      1,      1,    &pd140,  TYPONOFF,  ACC1,   NODEP,  &saf->md.wedler,      ""},
        { "Nadelbalken",                   1,      1,     41,      1,      2,      1,    &pd141,  TYPUCH,    ACC3,   NODEP,  &saf->md.nadelanzahl, ""},
        { "Tastenpieps",                   1,      1,     42,      0,      1,      1,    &pd142,  TYPONOFF,  ACC1,   NODEP,  &saf->md.tastenpiep,  ""},
        { "Fadenwedeln Fuss oben",         1,      1,     43,      0,      1,      1,    &pd143,  TYPONOFF,  ACC1,   NODEP,  &saf->md.wedelnfussoben, ""},
        { "Schablonendeckelueberw.",       1,      1,     44,      0,      1,      1,    &pd144,  TYPONOFF,  ACC1,   NODEP,  &nae.deckelueberwachung, ""},
        { "verkuerzter Anfangsfaden",      1,      1,     45,      0,      1,      1,    &pd145,  TYPONOFF,  ACC1,   NODEP,  &saf->md.kurzerAnffad, ""},
        { "Huepferfuss-Niveaus",           1,      1,     50, HFLEVEL_HUB_MIN, HFLEVEL_HUB_MAX, 9, &pd150, TYPUINT, ACC3, NODEP, &saf->md.hfniveau[0], "0,1mm"},
        { "Huepferfuss-Huebe",             1,      1,     51, HFLEVEL_HUB_MIN, HFLEVEL_HUB_MAX, 9, &pd151, TYPUINT, ACC3, NODEP, &saf->md.hfhub[0], "0,1mm"},
        { "Automatik Start",               1,      1,     52,      0,      1,      1,    &pd152,  TYPONOFF,  ACC1,   NODEP,  &saf->md.automatic,    ""},
        { "Konen Ueberw.",                 1,      1,     53,      0,      1,      1,    &pd153,  TYPONOFF,  ACC1,   NODEP,  &saf->md.koueberw,    ""},
        { "Vorhaltestiche Dreheinr.",      1,      1,     54,      0,      3,      1,    &pd154,  TYPUCH,    ACC1,   NODEP,  &saf->md.VorhStcheDreh, ""},    // SW015 
        
        
        
        
        // Name                            factor  group   number  min     max     size  default  type       access  depend  pointer               unit              (char)Invalid
        { "Stoffgegendruecker",            1,      2,      1,      0,      1,      1,    &pd201,  TYPUCH,    ACC3,   NODEP,  &saf->stoffgegendrueckerf, ""},
        { "Schablonenueberwachung",        1,      2,      2,      0,      3,      1,    &pd202,  TYPUCH,    ACC3,   NODEP,  &saf->jigctrf,        ""}, //SW005-1
        { "Maschinenvariante",             1,      2,      3,      0,      1,      1,    &pd203,  TYPUCH,    ACC3,   NODEP,  &saf->variante,       ""},
        { "Inkrement-Korrektur X-Motor",   1,      2,      6,    -30,     30,      1,    &pd206,  TYPCH,     ACC3,   NODEP,  &saf->inkr_korx,      "0,0001mm"},
        { "Inkrement-Korrektur Y-Motor",   1,      2,      7,    -30,     30,      1,    &pd207,  TYPCH,     ACC3,   NODEP,  &saf->inkr_kory,      "0,0001mm"},
        { "Schablonen-Korrektur",          1,      2,      8,    -30,     30,      1,    &pd208,  TYPCH,     ACC3,   NODEP,  &saf->schabl_kor,     "0,1mm"},
        
        // Name                            factor  group   number  min     max     size  default  type       access  depend  pointer               unit
        { "NIS",                           1,      3,      1,      0,      360,    1,    &pd301,  TYPINT,    ACC2,   NODEP,  &saf->md.nisvograd,   "Grad"},
        { "Fadenleger OT",                 1,      3,      2,      0,      360,    1,    &pd302,  TYPINT,    ACC2,   NODEP,  &saf->md.fadlot,      "Grad"},
        { "Fadenschneiden ein",            1,      3,      3,      0,      360,    1,    &pd303,  TYPINT,    ACC2,   NODEP,  &saf->md.ref2,        "Grad"},
        { "Fadenschneiden aus",            1,      3,      4,      0,      360,    1,    &pd304,  TYPINT,    ACC2,   NODEP,  &saf->md.y30offvograd,"Grad"},
        { "Fadspannung auf beim Schneiden",1,      3,      5,      0,      360,    1,    &pd305,  TYPINT,    ACC2,   NODEP,  &saf->md.fadspvograd, "Grad"},
        { "Fadspannung zu beim Ann„hen",   1,      3,      6,      0,      360,    1,    &pd306,  TYPINT,    ACC2,   NODEP,  &saf->md.spgzuvograd, "Grad"},
        
        // Name                            factor  group   number  min     max     size  default  type       access  depend  pointer               unit
        { "Fadenklemme auf",               1,      4,      1,      0,      256,    1,    &pd401,  TYPINT,    ACC2,   NODEP,  &saf->md.fadklemme_time, "10ms"},
        { "Fadenwedler",                   1,      4,      2,      0,      256,    1,    &pd402,  TYPINT,    ACC2,   NODEP,  &saf->md.fadwedler_time, "10ms"},
        { "Nadelkuehlung",                 1,      4,      3,      0,      256,    1,    &pd403,  TYPINT,    ACC2,   NODEP,  &saf->md.nadelkuehl_time,"10ms"},
        { "Zeit Fadenwedlerklemme auf",    1,      4,      4,      0,      256,    1,    &pd404,  TYPINT,    ACC2,   NODEP,  &saf->md.fwk_time,    "10ms"},
        //{ "Zeit Unterfadenklemme auf",     1,      4,      5,      0,      1000,   1,    &pd405,  TYPINT,    ACC2,   NODEP,  &saf->md.ufk_time,    "10ms"},
        { "Fadenzieher (Anfangsfaden)",    1,      4,      6,      0,      256,    1,    &pd406,  TYPINT,    ACC2,   NODEP,  &saf->md.fza_time,    "10ms"},
        { "Verz.st. Fadenwedlerklemme auf",1,      4,     10,      0,      99,     1,    &pd410,  TYPUCH,    ACC2,   NODEP,  &saf->md.fwk_verzst,  "St."},
        { "Verz.st. Unterfadenklemme auf", 1,      4,     11,      0,      99,     1,    &pd411,  TYPUCH,    ACC2,   NODEP,  &saf->md.ufk_verzst,  "St."},
        
        // Name                            factor  group   number  min     max     size  default  type       access  depend  pointer               unit
        { "Reduzierte Drehzahl",           100,    5,      1,     GSWMIN, GSWMAX,  1,    &pd501,  TYPCH,     ACC2,   NODEP,  &saf->md.redgsw,      "RPM"},
        { "Schneiddrehzahl",               10,     5,      2,      5,      25,     1,    &pd502,  TYPCH,     ACC2,   NODEP,  &saf->md.cutgsw,      "RPM"},
        { "Drehz. d. langsamen Anfangsst.",100,    5,      3,      2,      7,      1,    &pd503,  TYPCH,     ACC2,   NODEP,  &saf->md.annaetab[0], "RPM"},
        { "Anzahl d. langsamen Anfangsst.",1,      5,      4,      0,      99,     1,    &pd504,  TYPCH,     ACC2,   NODEP,  &saf->md.slowst,      "St."},
        /* EasyCODE > */
        /* EasyCODE < */
        // Name                            factor  group   number  min     max     size  default  type       access  depend  pointer               unit
        { "P-Anteil Drehzahlregler",       1,      7,      1,      1,      50,     1,    &pd701,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[1],   ""},
        { "I-Anteil Drehzahlregler",       1,      7,      2,      0,      100,    1,    &pd702,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[2],  ""},
        { "P-Anteil Lageregler",           1,      7,      3,      1,      100,    1,    &pd703,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[3],  ""},
        { "D-Anteil Lageregler",           1,      7,      4,      1,      100,    1,    &pd704,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[4],  ""},
        { "Zeit fuer Lagereglung",         1,      7,      5,      0,      100,    1,    &pd705,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[5],  "10ms"},
        { "P-Anteil Restbremse",           1,      7,      6,      1,      50,     1,    &pd706,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[6],  ""},
        { "Staerke der Restbremse",        1,      7,      7,      0,      100,    1,    &pd707,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[7],   ""},
        { "Positionierdrehzahl",           1,      7,      8,      3,      25,     1,    &pd708,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[8],   "10/min"},
        { "Beschleunigungsrampe",          1,      7,      9,      1,      50,     1,    &pd709,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[9],   "1/ms"},
        { "Bremsrampe",                    1,      7,      10,     1,      50,     1,    &pd710,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[10],  "1/ms"},
        { "Drehrichtung",                  1,      7,      11,     0,      1,      1,    &pd711,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[11],  ""},
        { "max. Motorstrom",               1,      7,      12,     7,      17,     1,    &pd712,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[12],  "A"},
        { "Positionierverfahren",          1,      7,      13,     1,      2,      1,    &pd713,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[13],  ""},
        { "Vorsteuerung Beschleunigen",    1,      7,      14,     0,     100,     1,    &pd714,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[14],  ""},
        { "Vorsteuerung Bremsen",          1,      7,      15,     0,     100,     1,    &pd715,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[15],  ""},
        { "Filter Drehzahlregler",         1,      7,      16,     1,       7,     1,    &pd716,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[16],  ""},
        { "Maschinenkennung",              1,      7,      17,     0,     255,     1,    &pd717,  TYPUCH,    ACC4,   NODEP,  &saf->md.quick_paramwert[17],  ""},
        
        // Name                            factor  group   number  min     max     size  default  type       access  depend  pointer               unit
        { "Parameter-Text anzeigen",       1,      9,      1,      0,      2,      1,    &pd901,  TYPUCH,    ACC4,   NODEP,  &eing.par.prtName,    ""},
        { "Zykluszeit",                    1,      9,      2,      0,      1000,   1,    &pd902,  TYPFLOAT,  ACC4,   NODEP,  &nae.zykluszeit,      "s"},
        { "Gesamtstueckzaehler",           1,      9,      3,      0,   99999999,  1,    &pd903,  TYPLONG,   ACC4,   NODEP,  &prod->total_piececount, "Stck."},
        { "Sicheres Fuss heben",           1,      9,      4,      0,      1,      1,    &pd904,  TYPONOFF,  ACC4,   NODEP,  &saf->md.hfsicher ,    ""},
        { "Quadrant 1 ein",                1,      9,      5,      1,     45,      1,    &pd905,  TYPUCH,    ACC4,   NODEP,  &saf->md.Nadeldreh1ein ,    "Grad"},
        { "Quadrant 4 ein",                1,      9,      6,      1,     80,      1,    &pd906,  TYPUCH,    ACC4,   NODEP,  &saf->md.Nadeldreh2ein ,    "Grad"},
        
        };
        /* EasyCODE > */
        /* EasyCODE ) */
        Init(parameter,sizeof (parameter)/sizeof (st_parameter));
        SetFreigabe(ACC1);
        parameterinit_enable = FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
grundzustand() */
/* EasyCODE F */
unsigned char KlassePar::grundzustand(void)
{
    unsigned char pressed,disp;
    while ( 1 )
    {
        /* EasyCODE < */
        pressed = keyb.taste();// gedruckte Taste merken
        /* EasyCODE > */
        switch ( pressed )
        {
        case TENDE :
            return E_EXIT;
        case TEINGABE :
            return /* EasyCODE < */
            E_GST;
            /* EasyCODE > */
            ;
        default:
            if ( koor.eingstop() )
            {
                return E_RUHE;
            }
            else
            {
                /* EasyCODE ( 0 
                ParTanz */
                switch ( key )
                {
                case 1 :
                    /* EasyCODE ( 0 
                    erste Aufruf (paint Display) */
                    /* EasyCODE < */
                    eing_botsch1.senden(BV008); // Display the main parameter display
                    /* EasyCODE > */
                    if ( CheckUp(TRUE) )
                    {
                        /* EasyCODE < */
                        // Die Werte sind nicht in ordnung !!
                        // KaltStart();// so werden alle Parameter reloaded
                        /* EasyCODE > */
                    }
                    if ( FindNextVisible() )
                    {
                        // Der erste sichbare Parameter ist
                        // gefunden und eingesetzt.Also weiter
                        lastKey = pressed;
                        teing1.lz(); // reset repeater timers
                        teing2.lz();
                        key = 3;
                    }
                    else
                    {
                        /* EasyCODE < */
                        // keine sichtbare Parameter ist gefunden
                        /* EasyCODE > */
                        return E_GST;
                    }
                    /* EasyCODE ) */
                    break;
                case 3 :
                    Invalidate(TRUE); // param anzeigen
                    key = 90;
                    break;
                case 90 :
                    /* EasyCODE ( 0 
                    is a new key pressed ? */
                    if ( lastKey == pressed )
                    {
                        if ( pressed==0xff )
                        {
                            /* EasyCODE < */
                            // released
                            /* EasyCODE > */
                        }
                        else
                        {
                            // go check first delay time
                            key = 91;
                        }
                    }
                    else
                    {
                        lastKey = pressed;
                        //disp = Operate(pressed);
                        Invalidate(Operate(pressed));
                        /* EasyCODE < */
                        teing1.rsz();
                        teing1.sz(saf->slowdelay); // delay for 1.st repeat
                        teing2.lz(); // stop repeater
                        /* EasyCODE > */
                    }
                    /* EasyCODE ) */
                    break;
                case 91 :
                    /* EasyCODE ( 0 
                    is first delay time over ? */
                    if ( lastKey == pressed )
                    {
                        if ( teing1.tz() )
                        {
                            /* EasyCODE < */
                            teing1.rsz();
                            // the last key will be slow repeated
                            // during this time
                            teing1.sz(saf->fastdelay);
                            teing2.rsz();
                            // repeater rate
                            teing2.sz(saf->slowtime);
                            Invalidate(Operate(pressed));
                            key = 92; // start slow repeat and wait to fast
                            /* EasyCODE > */
                        }
                    }
                    else
                    {
                        // Key is released before timeout
                        // or any other key is pressed
                        key = 90;
                        // zinvoll nur wenn andere Tastencode
                        // bevor 0xFF entdeckt wurde.
                        //Invalidate(Operate(pressed));
                        /* EasyCODE - */
                        lastKey = pressed;
                        Invalidate(Operate(pressed));
                    }
                    /* EasyCODE ) */
                    break;
                case 92 :
                    /* EasyCODE ( 0 
                    repeat key slow */
                    if ( lastKey == pressed )
                    {
                        if ( teing1.tz() )
                        {
                            // second delay time is over
                            // new time for fast repeat
                            teing1.rsz();
                            teing1.sz(200);
                            teing2.rsz();
                            teing2.sz(saf->fasttime);
                            Invalidate(Operate(pressed));
                            key = 93;
                        }
                        else
                        {
                            if ( teing2.tz() )
                            {
                                // repeat slow
                                teing2.rsz();
                                teing2.sz(saf->slowtime);
                                Invalidate(Operate(pressed));
                            }
                        }
                    }
                    else
                    {
                        // key is released
                        key = 90;
                        Invalidate(Operate(pressed));
                    }
                    /* EasyCODE ) */
                    break;
                case 93 :
                    /* EasyCODE ( 0 
                    repeat key fast */
                    if ( lastKey == pressed )
                    {
                        if ( teing1.tz() )
                        {
                            // delay for very fast
                            // is over
                            teing2.rsz();
                            teing2.sz(saf->fasttime);
                            Invalidate(Operate(pressed));
                            key = 94;
                        }
                        else
                        {
                            if ( teing2.tz() )
                            {
                                // repeat fast
                                teing2.rsz();
                                teing2.sz(saf->fasttime);
                                Invalidate(Operate(pressed));
                            }
                        }
                    }
                    else
                    {
                        // The key is released
                        // goto first checkpoint
                        key = 90;
                        Invalidate(Operate(pressed));
                    }
                    /* EasyCODE ) */
                    break;
                case 94 :
                    /* EasyCODE ( 0 
                    repeat key very fast */
                    if ( lastKey == pressed )
                    {
                        if ( teing2.tz() )
                        {
                            // repeat fast
                            teing2.rsz();
                            teing2.sz(saf->fasttime);
                            Operate(pressed);
                            Operate(pressed);
                            Operate(pressed);
                            Operate(pressed);
                            Invalidate(Operate(pressed));
                        }
                    }
                    else
                    {
                        // The key is released
                        // goto first checkpoint
                        key = 90;
                        Invalidate(Operate(pressed));
                    }
                    /* EasyCODE ) */
                    break;
                case 0 :
                    key = 1;
                    break;
                }
                /* EasyCODE ) */
            }
        }
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Set's */
/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
SetPar(number,n,src) EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
void KlassePar::SetPar(unsigned int number,void* src,unsigned char n)
{
    // Sets the given parameter to given value
    // number: parameter number
    // n: index of parameter (if parameter is an array) default=0
    // src: parameter will be loaded from this pointer
    void* dest = GetParPtr(number,n);
    switch ( GetParType(number) )
    {
    case TYPCH :
        *(char*)dest = *(char*)src;
        break;
    case TYPLONG :
        *(long*)dest = *(long*)src;
        break;
    case TYPINT :
        *(int*)dest = *(int*)src;
        break;
    case TYPFLOAT :
        *(float*)dest = *(float*)src;
        break;
    case TYPDBL :
        *(double*)dest = *(double*)src;
        break;
    case TYPBOOL :
        *(char*)dest = *(char*)src;
        break;
    case TYPONOFF :
        *(char*)dest = *(char*)src;
        break;
    case TYPUPDN :
        *(char*)dest = *(char*)src;
        break;
    case TYPUNDEF :
        return ;
    default:
        return ;
    }
    Norm();
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ( 0 
SetFreigabe(int level) */
/* EasyCODE F */
char KlassePar::SetFreigabe(char level)
{
    // set the access level of object and returns the old level
    char old = freigabe ;
    freigabe = level ;
    return old;
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Get's */
/* EasyCODE ( 0 
GetParPtr(number,n) */
/* EasyCODE F */
void* KlassePar::GetParPtr(unsigned int number,unsigned char n)
{
    // return a void pointer to parameter (not struct pointer!)
    // number: parameter number
    // n: array index (only if parameter is an array) default = 0
    void* ptr = GetStructPointer(number)->pVar;
    if ( n > GetStructPointer(number)->size-1 )
    {
        // refer to last member if n greather than max
        n = GetStructPointer(number)->size-1;
    }
    switch ( GetParType(number) )
    {
    case TYPCH :
        /* EasyCODE = */
    case TYPBOOL :
        /* EasyCODE = */
    case TYPONOFF :
        /* EasyCODE = */
    case TYPUPDN :
        /* EasyCODE = */
    case TYPUCH :
        (char*)ptr+=n;
        break;
    case TYPUINT :
        /* EasyCODE = */
    case TYPINT :
        (int*)ptr+=n;
        break;
    case TYPFLOAT :
        (float*)ptr+=n;
        break;
    case TYPDBL :
        (double*)ptr+=n;
        break;
    case TYPLONG :
        (long*)ptr+=n;
        break;
    case TYPUNDEF :
        break;
    }
    return ptr;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetParType(number) */
/* EasyCODE F */
char KlassePar::GetParType(unsigned int number)
{
    /* EasyCODE < */
    // returns the type of parameter. char , int , dbl ,.....
    /* EasyCODE > */
    return GetStructPointer(number)->typ;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetStructPointer(number) */
/* EasyCODE F */
st_parameter* KlassePar::GetStructPointer(unsigned int number)
{
    /* EasyCODE < */
    // points to parameter structure
    // number: parameter number
    // returns Zero if parameter with given number not exist.
    st_parameter* ptr = pSt;
    int index = GetParIndex(number); // physical index of parameter
    /* EasyCODE > */
    if ( index == NOMATCH )
    {
        // no such a parameter exist
        ptr = 0;
    }
    else
    {
        // point to requested par. structure
        ptr+=index;
    }
    return ptr;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetParIndex(number) */
/* EasyCODE F */
int KlassePar::GetParIndex(unsigned int number)
{
    // returns the index of parameter in st_parameter structure array
    // number: parameter number
    // returns NOMATCH (-1) if not exist
    int group = number/100;
    int index = number - group*100;
    st_parameter* ptr = pSt;
    for ( int i = 0; i < size ; i++ )
    {
        if ( ptr->group == group )
        {
            /* EasyCODE < */
            // within group
            /* EasyCODE > */
            if ( index == ptr->index )
            {
                return i;
            }
        }
        ptr++;
    }
    return NOMATCH;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetSize() */
/* EasyCODE F */
int KlassePar::GetSize()
{
    /* EasyCODE < */
    // returns the number of parameters in this object
    /* EasyCODE > */
    return size;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetArraySize(number) */
/* EasyCODE F */
int KlassePar::GetArraySize(unsigned int number)
{
    /* EasyCODE < */
    // returns the size of array from given parameter number
    /* EasyCODE > */
    return GetStructPointer(number)->size;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetValStr(number,pStr,n) */
/* EasyCODE F */
char KlassePar::GetValStr(unsigned int number,char* pStr,unsigned char n)
{
    // fills the pStr with the ASCII code of given parameter.This location must be enough long !
    // number is the parameter number
    // n is the index of parameter array if parameter is an array. Default value is zero.
    // return: number of characters in Null terminated string (inc. Null).
    char len = 1;
    void* p = GetParPtr(number,n);
    int faktor = GetStructPointer(number)->faktor;
    switch ( GetParType(number) )
    {
    case TYPUCH :
        sprintf(pStr,"%d",*(unsigned char*)p * faktor);
        break;
    case TYPCH :
        sprintf(pStr,"%d",*(char*)p * faktor);
        break;
    case TYPLONG :
        sprintf(pStr,"%ld",*(long*)p * faktor);
        break;
    case TYPUINT :
        sprintf(pStr,"%d",*(unsigned int*)p * faktor);
        break;
    case TYPINT :
        sprintf(pStr,"%d",*(int*)p * faktor);
        break;
    case TYPFLOAT :
        sprintf(pStr,"%.3f",*(float*)p * faktor);
        break;
    case TYPDBL :
        sprintf(pStr,"%.3f",*(double*)p * faktor);
        break;
    case TYPBOOL :
        if ( *(char*)p == 0 )
        {
            sprintf(pStr,"FALSE");
        }
        else
        {
            sprintf(pStr,"TRUE");
        }
        break;
    case TYPONOFF :
        if ( *(char*)p == 0 )
        {
            sprintf(pStr,"OFF");
        }
        else
        {
            sprintf(pStr,"ON");
        }
        break;
    case TYPUPDN :
        if ( *(char*)p == 0 )
        {
            sprintf(pStr,"DOWN");
        }
        else
        {
            sprintf(pStr,"UP");
        }
        break;
    case TYPUNDEF :
        sprintf(pStr,"????");
        break;
    default:
        break;
    }
    while ( *pStr != 0 )
    {
        // number of characters in string.
        len++;
        pStr++;
    }
    return len;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetUnitStr(number,pStr) */
/* EasyCODE F */
char KlassePar::GetUnitStr(unsigned int number,char* pStr)
{
    // fills the pStr with the ASCII code of parameter unit.This location must be enough long !
    // number is the parameter number
    // return: number of characters in Null terminated string (inc. Null).
    char len = 1;
    st_parameter* p = GetStructPointer(number);
    sprintf(pStr,"%s",p->pUnit);
    while ( *pStr != 0 )
    {
        // number of characters in string.
        len++;
        pStr++;
    }
    return len;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetFreigabe(void) */
/* EasyCODE F */
char KlassePar::GetFreigabe(void)
{
    /* EasyCODE < */
    // es meldet die Fraigabe von Object zurueck.(nicht vom Parameter!)
    /* EasyCODE > */
    return freigabe;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetNextGroup(istGrp) */
/* EasyCODE F */
char KlassePar::GetNextGroup(char istGrp)
{
    // find next valid group and return it
    // return -1 if not found
    // return istGrp if only one group exist
    st_parameter* ptr;
    char istFound = 0;
    while ( 1 )
    {
        /* EasyCODE < */
        ptr = pSt; // copy a pointer to par array
        /* EasyCODE > */
        for ( int i = 0 ; i < size ; i++ )
        {
            if ( istFound )
            {
                if ( ptr->group > istGrp )
                {
                    if ( IsFreeToShow(ptr) )
                    {
                        return ptr->group;
                    }
                }
            }
            else
            {
                if ( ptr->group == istGrp )
                {
                    istFound = 1;
                }
            }
            ptr++;
        }
        return -1;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetPrevGroup(istGrp) */
/* EasyCODE F */
char KlassePar::GetPrevGroup(char istGrp)
{
    // find next valid group and return it
    // return -1 if not found
    // return istGrp if only one group exist
    st_parameter* ptr;// copy a pointer to par array
    char istFound = 0;
    while ( 1 )
    {
        /* EasyCODE < */
        ptr = pSt; // copy a pointer to par array
        ptr+= (size-1); // point to last element
        /* EasyCODE > */
        for ( int i = size-1 ; i >=0  ; i-- )
        {
            if ( istFound )
            {
                if ( ptr->group < istGrp )
                {
                    if ( IsFreeToShow(ptr) )
                    {
                        return ptr->group;
                    }
                }
            }
            else
            {
                if ( ptr->group == istGrp )
                {
                    istFound = 1;
                }
            }
            ptr--;
        }
        return -1;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetNextIndex(istGroup,istIndex) */
/* EasyCODE F */
char KlassePar::GetNextIndex(char istGroup,char istIndex)
{
    // find next valid Index and return it
    // return -1 if not found
    st_parameter* ptr;
    char grpFound = 0;
    while ( 1 )
    {
        /* EasyCODE < */
        ptr = pSt; // copy a pointer to par struct array
        /* EasyCODE > */
        for ( int i = 0 ; i < size ; i++ )
        {
            if ( grpFound )
            {
                if ( ptr->group == istGroup )
                {
                    if ( ptr->index > istIndex )
                    {
                        if ( IsFreeToShow(ptr) )
                        {
                            return ptr->index;
                        }
                    }
                    else
                    {
                        /* EasyCODE < */
                        // next
                        /* EasyCODE > */
                    }
                }
                else
                {
                    /* EasyCODE < */
                    // Group change !
                    /* EasyCODE > */
                    return -1;
                }
            }
            else
            {
                if ( ptr->group == istGroup )
                {
                    grpFound = 1;
                }
            }
            ptr++;
        }
        return -1;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetPrevIndex(istGroup,istIndex) */
/* EasyCODE F */
char KlassePar::GetPrevIndex(char istGroup,char istIndex)
{
    // find next valid Index and return it
    // return -1 if not found
    st_parameter* ptr;
    char grpFound = 0;
    while ( 1 )
    {
        /* EasyCODE < */
        ptr = pSt; // copy a pointer to par array
        ptr+= (size-1); // point to last element
        /* EasyCODE > */
        for ( int i = size-1 ; i >=0  ; i-- )
        {
            if ( grpFound )
            {
                if ( ptr->group == istGroup )
                {
                    if ( ptr->index < istIndex )
                    {
                        if ( IsFreeToShow(ptr) )
                        {
                            return ptr->index;
                        }
                    }
                    else
                    {
                        /* EasyCODE < */
                        // next
                        /* EasyCODE > */
                    }
                }
                else
                {
                    /* EasyCODE < */
                    // Group change !
                    /* EasyCODE > */
                    return -1;
                }
            }
            else
            {
                if ( ptr->group == istGroup )
                {
                    grpFound = 1;
                }
            }
            ptr--;
        }
        return -1;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetMaxIndexInGroup(istGroup) */
/* EasyCODE F */
char KlassePar::GetMaxIndexInGroup(char istGroup)
{
    // find max index number in the given group
    // return 0 if not exist
    st_parameter* ptr;
    char grpFound = 0;
    char result = 0;
    /* EasyCODE < */
    ptr = pSt; // copy a pointer to par array
    /* EasyCODE > */
    for ( int i = 0 ; i < size ; i++ )
    {
        if ( grpFound )
        {
            /* EasyCODE ( 0 
            search max. index */
            if ( ptr->group == istGroup )
            {
                if ( ptr->index > result )
                {
                    if ( IsFreeToShow(ptr) )
                    {
                        result = ptr->index;
                    }
                }
                else
                {
                    /* EasyCODE < */
                    // next
                    /* EasyCODE > */
                }
            }
            else
            {
                /* EasyCODE < */
                // Group change !
                /* EasyCODE > */
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE ( 0 
            search to find given group */
            if ( ptr->group == istGroup )
            {
                grpFound = 1;
                if ( IsFreeToShow(ptr) )
                {
                    result = ptr->index;
                }
            }
            /* EasyCODE ) */
        }
        ptr++;
    }
    return result;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetMinIndexInGroup(istGroup) */
/* EasyCODE F */
char KlassePar::GetMinIndexInGroup(char istGroup)
{
    // find min index number in the given group
    // return 0 if group not exist
    st_parameter* ptr;
    char grpFound = 0;
    char result = 100;
    /* EasyCODE < */
    ptr = pSt; // copy a pointer to par array
    /* EasyCODE > */
    for ( int i = 0 ; i < size ; i++ )
    {
        if ( grpFound )
        {
            /* EasyCODE ( 0 
            search min. index */
            if ( ptr->group == istGroup )
            {
                if ( ptr->index < result )
                {
                    if ( IsFreeToShow(ptr) )
                    {
                        result = ptr->index;
                    }
                }
                else
                {
                    /* EasyCODE < */
                    // next
                    /* EasyCODE > */
                }
            }
            else
            {
                /* EasyCODE < */
                // Group change !
                /* EasyCODE > */
            }
            /* EasyCODE ) */
        }
        else
        {
            /* EasyCODE ( 0 
            search to find the given group */
            if ( ptr->group == istGroup )
            {
                grpFound = 1;
                if ( IsFreeToShow(ptr) )
                {
                    result = ptr->index;
                }
            }
            /* EasyCODE ) */
        }
        ptr++;
    }
    return result;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
double GetUpperLimit(int number) */
/* EasyCODE F */
double KlassePar::GetUpperLimit(int number)
{
    double dest;
    st_parameter* ptr = GetStructPointer(number);
    if ( ptr->ref & MAXDEP )
    {
        if ( GetParVal(ptr->max,&dest) )
        {
        }
        else
        {
            /* EasyCODE < */
            // Error
            /* EasyCODE > */
        }
    }
    else
    {
        dest = ptr->max;
    }
    return dest;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
double GetLowerLimit(int number) */
/* EasyCODE F */
double KlassePar::GetLowerLimit(int number)
{
    double dest;
    st_parameter* ptr = GetStructPointer(number);
    if ( ptr->ref & MINDEP )
    {
        if ( GetParVal(ptr->min,&dest) )
        {
        }
        else
        {
            /* EasyCODE < */
            // Error
            /* EasyCODE > */
        }
    }
    else
    {
        dest = ptr->min;
    }
    return dest;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
double GetDefault(int number,int n) */
/* EasyCODE F */
double KlassePar::GetDefault(int number,int n)
{
    // number is the parameter number
    // n is the index if parameter is an array. default = 0;
    // return the actual default value
    st_parameter* ptr = GetStructPointer(number);
    return GetDefault(ptr,n);
}

/* EasyCODE ) */
/* EasyCODE ( 0 
double GetDefault(st_parameter* ptr,int n) */
/* EasyCODE F */
double KlassePar::GetDefault(st_parameter* ptr,int n)
{
    // ptr is the pointer to parameter structure
    // n is the index if parameter is an array. default = 0;
    // return the actual default value. (DEFDEP==TRUE)?
    double dest;
    if ( ptr->ref & DEFDEP )
    {
        // Default value defined from others.
        int num = (int)GetParDef(ptr,&dest,n);
        if ( GetParVal(ptr->group*100+num , &dest) )
        {
        }
        else
        {
            /* EasyCODE < */
            // Error
            // no such a parameter exist
            /* EasyCODE > */
        }
    }
    else
    {
        // deafault value from Table
        GetParDef(ptr,&dest,n);
    }
    return dest;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetParVal(number,&dest,n) */
/* EasyCODE F */
char KlassePar::GetParVal(unsigned int number,double* dest,unsigned char n)
{
    // parameter value will be loaded in dest without factor multiplication.
    // number is the parameter number
    // n is the index of parameter array if parameter is an array. Default value is zero.
    // return TRUE if successful.
    char result=TRUE;
    void* p = GetParPtr(number,n);
    st_parameter* ptr = GetStructPointer(number);
    if ( ptr )
    {
        /* EasyCODE ( 0 
        pointer is valid */
        switch ( ptr->typ )
        {
        case TYPCH :
            *dest = *(char*)p;
            break;
        case TYPLONG :
            *dest = *(long*)p;
            break;
        case TYPUCH :
            *dest = *(unsigned char*)p;
            break;
        case TYPINT :
            *dest = *(int*)p;
            break;
        case TYPFLOAT :
            *dest = *(float*)p;
            break;
        case TYPDBL :
            *dest = *(double*)p;
            break;
        case TYPBOOL :
            *dest = *(char*)p;
            break;
        case TYPONOFF :
            *dest = *(char*)p;
            break;
        case TYPUPDN :
            *dest = *(char*)p;
            break;
        case TYPUNDEF :
            // Error
            result = FALSE;
            break;
        default:
            // Error
            result = FALSE;
            break;
        }
        return result;
        /* EasyCODE ) */
    }
    else
    {
        return FALSE;
    }
}

/* EasyCODE_STRUCTURED_COMMENT_START */
/*
EasyCODE_COMMENT_START EasyCODE ( 0 
GetParVal(number,&dest,n) improved ! but not tested EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
// as improved, not tested !
EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE F EasyCODE_COMMENT_END
EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
char KlassePar::GetParVal(unsigned int number,double* dest,unsigned char n)
EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
{
    EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
    // parameter value will be loaded in dest without factor multiplication.
    // number is the parameter number
    // n is the index of parameter array if parameter is an array. Default value is zero.
    // return TRUE if successful.
    //char result=TRUE;
    void* p = GetParPtr(number,n);
    st_parameter* ptr = GetStructPointer(number);
    EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
    if ( ptr )
    {
        return EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
        GetPar(p,ptr->typ,dest)
        EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
        ;
    }
    else
    {
        return EasyCODE_COMMENT_START EasyCODE < EasyCODE_COMMENT_END
        FALSE
        EasyCODE_COMMENT_START EasyCODE > EasyCODE_COMMENT_END
        ;
    }
}

EasyCODE_COMMENT_START EasyCODE ) EasyCODE_COMMENT_END
*/
/* EasyCODE_STRUCTURED_COMMENT_END */
/* EasyCODE ) */
/* EasyCODE ( 0 
GetPar(ptr,type,dest,n) */
/* EasyCODE F */
char KlassePar::GetPar(void* p,int type,double* dest,int n)
{
    // parameter value will be loaded in dest without factor multiplication.
    // p is the pointer to parameter
    // type is the type of parameter
    // n is the index of parameter array if parameter is an array. Default value is zero.
    // return TRUE if successful.
    char result = TRUE;
    //void* p = ptr->pVar;
    switch ( type )
    {
    case TYPCH :
        *dest = *((char*)p+n);
        break;
    case TYPLONG :
        *dest = *((long*)p+n);
        break;
    case TYPUCH :
        *dest = *((unsigned char*)p+n);
        break;
    case TYPINT :
        *dest = *((int*)p+n);
        break;
    case TYPFLOAT :
        *dest = *((float*)p+n);
        break;
    case TYPDBL :
        *dest = *((double*)p+n);
        break;
    case TYPBOOL :
        *dest = *((char*)p+n);
        break;
    case TYPONOFF :
        *dest = *((char*)p+n);
        break;
    case TYPUPDN :
        *dest = *((char*)p+n);
        break;
    case TYPUNDEF :
        // Error
        result = FALSE;
        break;
    default:
        // Error
        result = FALSE;
        break;
    }
    return result;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetParDef(number,&dest,n) */
/* EasyCODE F */
char KlassePar::GetParDef(unsigned int number,double* dest,unsigned char n)
{
    // default value from table will be loaded in dest without any change.
    // number is the parameter number
    // n is the index of parameter array if parameter is an array. Default value is zero.
    // return TRUE if successful.
    st_parameter* ptr = GetStructPointer(number);
    if ( ptr )
    {
        return GetParDef(ptr,dest,n);
    }
    else
    {
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
GetParDef(st_parameter* ptr,&dest,n) */
/* EasyCODE F */
char KlassePar::GetParDef(st_parameter* ptr,double* dest,unsigned char n)
{
    // default value from table will be loaded in 'dest' without any change.
    // ptr is the parameter structure address
    // n is the index of parameter array if parameter is an array. Default value is zero.
    // return TRUE if successful.
    char result=TRUE;
    if ( ptr )
    {
        /* EasyCODE ( 0 
        pointer is valid */
        const void* p = ptr->def;
        switch ( ptr->typ )
        {
        case TYPCH :
            (char*)p+=n;
            *dest = *(char*)p;
            break;
        case TYPLONG :
            (long*)p+=n;
            *dest = *(long*)p;
            break;
        case TYPUCH :
            (unsigned char*)p+=n;
            *dest = *(unsigned char*)p;
            break;
        case TYPINT :
            (int*)p+=n;
            *dest = *(int*)p;
            break;
        case TYPFLOAT :
            (float*)p+=n;
            *dest = *(float*)p;
            break;
        case TYPDBL :
            (double*)p+=n;
            *dest = *(double*)p;
            break;
        case TYPBOOL :
            (char*)p+=n;
            *dest = *(char*)p;
            break;
        case TYPONOFF :
            (char*)p+=n;
            *dest = *(char*)p;
            break;
        case TYPUPDN :
            (char*)p+=n;
            *dest = *(char*)p;
            break;
        case TYPUNDEF :
            // Error
            result = FALSE;
            break;
        default:
            // Error
            result = FALSE;
            break;
        }
        return result;
        /* EasyCODE ) */
    }
    else
    {
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ( 0 
Others */
/* EasyCODE ( 0 
IncPar(number,n,grew) */
/* EasyCODE F */
void KlassePar::IncPar(unsigned int number,unsigned char n,double grew)
{
    // increment Parameter
    // n is index of par[n] if parameter is an array
    long maxVal = GetUpperLimit(number);
    
    void* p = GetParPtr(number,n);
    int intGrew = (int)grew;
    char chGrew = (char)grew;
    float fGrew = (float)grew;
    long  lGrew = (long)grew;
    if ( IsFreeToEdit(GetStructPointer(number)) )
    {
        GetStructPointer(number)->Invalid = TRUE;
        /* EasyCODE ( 0 
        Increment */
        if ( intGrew == 0 )
        {
            intGrew = 1;
            chGrew = 1;
            lGrew = 1;
        }
        switch ( GetParType(number) )
        {
        case TYPUCH :
            if ( (*(unsigned char*)p + chGrew) < maxVal )
            {
                *(unsigned char*)p = (*(unsigned char*)p) + chGrew ;
            }
            else
            {
                *(unsigned char*)p = maxVal ;
            }
            break;
        case TYPCH :
            if ( (*(char*)p + chGrew) < maxVal )
            {
                *(char*)p = (*(char*)p) + chGrew ;
            }
            else
            {
                *(char*)p = maxVal ;
            }
            break;
        case TYPLONG :
            if ( (*(long*)p + lGrew) < maxVal )
            {
                *(long*)p = (*(long*)p) + lGrew ;
            }
            else
            {
                *(long*)p = maxVal ;
            }
            break;
        case TYPUINT :
            if ( (*(unsigned int*)p + intGrew) < maxVal )
            {
                *(unsigned int*)p = (*(unsigned int*)p) + intGrew;
            }
            else
            {
                *(unsigned int*)p = maxVal;
            }
            break;
        case TYPINT :
            if ( (*(int*)p + intGrew) < maxVal )
            {
                *(int*)p = (*(int*)p) + intGrew;
            }
            else
            {
                *(int*)p = maxVal;
            }
            break;
        case TYPFLOAT :
            if ( (*(float*)p + fGrew) < maxVal )
            {
                *(float*)p = (*(float*)p) + fGrew ;
            }
            else
            {
                *(float*)p = maxVal ;
            }
            break;
        case TYPDBL :
            if ( (*(double*)p + grew) < maxVal )
            {
                *(double*)p = (*(double*)p) + grew ;
            }
            else
            {
                *(double*)p = maxVal ;
            }
            break;
        case TYPBOOL :
            if ( *(char*)p == 0 )
            {
                *(char*)p = 1 ;
            }
            break;
        case TYPONOFF :
            if ( *(char*)p == 0 )
            {
                *(char*)p = 1 ;
            }
            break;
        case TYPUPDN :
            if ( *(char*)p == 0 )
            {
                *(char*)p = 1 ;
            }
            break;
        case TYPUNDEF :
            break;
        }
        /* EasyCODE ) */
    }
    else
    {
        // change not allowed
        fkeytst(200);
        key = 1;
        return ;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
DecPar(number,n,grew) */
/* EasyCODE F */
void KlassePar::DecPar(unsigned int number,unsigned char n,double grew)
{
    /* EasyCODE < */
    // Decrement Parameter
    // n is index of par[n] if parameter is an array
    /* EasyCODE > */
    GetStructPointer(number)->Invalid = TRUE;
    long minVal = GetLowerLimit(number);
    void* p = GetParPtr(number,n);
    int intGrew = (int)grew;
    char chGrew = (char)grew;
    float fGrew = (float)grew;
    long  lGrew = (long)grew;
    if ( IsFreeToEdit(GetStructPointer(number)) )
    {
        GetStructPointer(number)->Invalid = TRUE;
        /* EasyCODE ( 0 
        Decrement */
        if ( intGrew == 0 )
        {
            intGrew = 1;
            chGrew = 1;
            lGrew = 1;
        }
        switch ( GetParType(number) )
        {
        case TYPUCH :
            if ( (*(unsigned char*)p - chGrew) > minVal )
            {
                *(unsigned char*)p = (*(unsigned char*)p) - chGrew ;
            }
            else
            {
                *(unsigned char*)p = minVal ;
            }
            break;
        case TYPCH :
            if ( (*(char*)p - chGrew) > minVal )
            {
                *(char*)p = (*(char*)p) - chGrew ;
            }
            else
            {
                *(char*)p = minVal ;
            }
            break;
        case TYPLONG :
            if ( (*(long*)p - lGrew) > minVal )
            {
                *(long*)p = (*(long*)p) - lGrew ;
            }
            else
            {
                *(long*)p = minVal ;
            }
            break;
        case TYPUINT :
            if ( (*(unsigned int*)p - intGrew) > minVal )
            {
                *(unsigned int*)p = (*(unsigned int*)p) - intGrew;
            }
            else
            {
                *(unsigned int*)p = minVal;
            }
            break;
        case TYPINT :
            if ( (*(int*)p - intGrew) > minVal )
            {
                *(int*)p = (*(int*)p) - intGrew;
            }
            else
            {
                *(int*)p = minVal;
            }
            break;
        case TYPFLOAT :
            if ( (*(float*)p - fGrew) > minVal )
            {
                *(float*)p = (*(float*)p) - fGrew ;
            }
            else
            {
                *(float*)p = minVal ;
            }
            break;
        case TYPDBL :
            if ( (*(double*)p - grew) > minVal )
            {
                *(double*)p = (*(double*)p) - grew ;
            }
            else
            {
                *(double*)p = minVal ;
            }
            break;
        case TYPBOOL :
            if ( *(char*)p != 0 )
            {
                *(char*)p = 0 ;
            }
            break;
        case TYPONOFF :
            if ( *(char*)p != 0 )
            {
                *(char*)p = 0 ;
            }
            break;
        case TYPUPDN :
            if ( *(char*)p != 0 )
            {
                *(char*)p = 0 ;
            }
            break;
        case TYPUNDEF :
            break;
        }
        /* EasyCODE ) */
    }
    else
    {
        // change not allowed
        fkeytst(200);
        key = 1;
        return ;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
IsChanged(number) */
/* EasyCODE F */
char KlassePar::IsChanged(unsigned int number)
{
    /* EasyCODE < */
    // Ob ein Parameter geaendert ist
    // number: Parameter Nummer
    // return: TRUE if geaendert
    /* EasyCODE > */
    return GetStructPointer(number)->Invalid;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
DelChangedInfo(number) */
/* EasyCODE F */
void KlassePar::DelChangedInfo(unsigned int number)
{
    /* EasyCODE < */
    // Info ob ein Parameter geaendert ist löschen
    // number: Parameter Nummer
    /* EasyCODE > */
    GetStructPointer(number)->Invalid = FALSE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
KaltStart(void) */
/* EasyCODE F */
char KlassePar::KaltStart(void)
{
    /* EasyCODE < */
    // load all parameters from their default values
    /* EasyCODE > */
    st_parameter* ptr = pSt; // copy a pointer to par array
    double dest;
    for ( int i = 0 ; i < size ; i++ )
    {
        if ( ((ptr->group==9) && (ptr->index==3)) ||
             (ptr->group==2) )
        {
        }
        else
        {
            LoadDefault(ptr);
        }
        ptr++;
    }
    return 0;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
CheckUp(correction) */
/* EasyCODE < */
// checks all parameter values.
// loads default value if corr is TRUE for any parameter that is out of range
// and return value is none zero if at least one parameter
// has been out of range.
// ** Only the out of range parameters will be loaded **
// you can reload all parameters(KaltStart()) upon return value.
/* EasyCODE > */
/* EasyCODE F */
char KlassePar::CheckUp(char corr)
{
    st_parameter* ptr = pSt; // copy a pointer to par array
    const void* src;
    void* dst;
    int j;
    char result = 0;
    for ( int i = 0 ; i < size ; i++ )
    {
        /* EasyCODE < */
        dst = ptr->pVar; // address of parameter
        src = ptr->def; // address of default values
        /* EasyCODE > */
        switch ( ptr->typ )
        {
        case TYPCH :
            /* EasyCODE = */
        case TYPBOOL :
            /* EasyCODE = */
        case TYPONOFF :
            /* EasyCODE = */
        case TYPUPDN :
            /* EasyCODE = */
        case TYPUCH :
            /* EasyCODE ( 0 
            BOOLean und char parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(char*)dst > *(char*)GetParPtr(ptr->max,0) )
                    {
                        // max value is depend on another Par.
                        // max is the par no.
                        result = 1;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(char*)dst > ptr->max )
                    {
                        result = 2;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(char*)dst < *(char*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        result = 3;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(char*)dst < ptr->min )
                    {
                        result = 4;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((char*)dst)++;
                ((char*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPFLOAT :
            /* EasyCODE ( 0 
            float parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(float*)dst > *(float*)GetParPtr(ptr->max,0) )
                    {
                        /* EasyCODE < */
                        // max value is depend on another Par.
                        // max is the par no.
                        /* EasyCODE > */
                        result = 5;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(float*)dst > ptr->max )
                    {
                        result = 6;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(float*)dst < *(float*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        result = 7;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(float*)dst < ptr->min )
                    {
                        result = 8;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((float*)dst)++;
                ((float*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPDBL :
            /* EasyCODE ( 0 
            double type parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(double*)dst > *(double*)GetParPtr(ptr->max,0) )
                    {
                        /* EasyCODE < */
                        // max value is depend on another Par.
                        // max is the par no.
                        /* EasyCODE > */
                        result = 9;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(double*)dst > ptr->max )
                    {
                        result = 10;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(double*)dst <  *(double*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        result = 11;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(double*)dst < ptr->min )
                    {
                        result = 12;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((double*)dst)++;
                ((double*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPUINT :
            /* EasyCODE = */
        case TYPINT :
            /* EasyCODE ( 0 
            int und uint type parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(int*)dst > *(int*)GetParPtr(ptr->max,0) )
                    {
                        /* EasyCODE < */
                        // max value is depend on another Par.
                        // max is the par no.
                        /* EasyCODE > */
                        result = 13;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(int*)dst > ptr->max )
                    {
                        result = 14;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(int*)dst < *(int*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        result = 15;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(int*)dst < ptr->min )
                    {
                        result = 16;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((int*)dst)++;
                ((int*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPLONG :
            /* EasyCODE ( 0 
            long type parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(long*)dst > *(long*)GetParPtr(ptr->max,0) )
                    {
                        /* EasyCODE < */
                        // max value is depend on another Par.
                        // max is the par no.
                        /* EasyCODE > */
                        result = 13;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(long*)dst > ptr->max )
                    {
                        result = 14;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(long*)dst < *(long*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        result = 15;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(long*)dst < ptr->min )
                    {
                        result = 16;
                        if ( corr )
                        {
                            LoadDefault(ptr);
                        }
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((long*)dst)++;
                ((long*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPUNDEF :
            break;
        }
        ptr++;
    }
    return result;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Norm() */
/* EasyCODE < */
// checks all parameter values.
// if greather than max then = max
// if less than min then = min
// and return value is none zero if at least one parameter
// has been out of range.
/* EasyCODE > */
/* EasyCODE F */
char KlassePar::Norm()
{
    st_parameter* ptr = pSt; // copy a pointer to par array
    const void* src;
    void* dst;
    int j;
    char result = 0;
    for ( int i = 0 ; i < size ; i++ )
    {
        /* EasyCODE < */
        dst = ptr->pVar; // address of parameter
        src = ptr->def; // address of default values
        /* EasyCODE > */
        switch ( ptr->typ )
        {
        case TYPCH :
            /* EasyCODE = */
        case TYPBOOL :
            /* EasyCODE = */
        case TYPONOFF :
            /* EasyCODE = */
        case TYPUPDN :
            /* EasyCODE ( 0 
            BOOLean und char parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(char*)dst > *(char*)GetParPtr(ptr->max,0) )
                    {
                        // max value is depend on another Par.
                        // max is the par no.
                        result = 1;
                        *(char*)dst = *(char*)GetParPtr(ptr->max,0);
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(char*)dst > ptr->max )
                    {
                        *(char*)dst = ptr->max;
                        result = 2;
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(char*)dst < *(char*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        *(char*)dst = *(char*)GetParPtr(ptr->min,0);
                        result = 3;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(char*)dst < ptr->min )
                    {
                        *(char*)dst = ptr->min;
                        result = 4;
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((char*)dst)++;
                ((char*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPUCH :
            /* EasyCODE ( 0 
            unsigned char parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(unsigned char*)dst > *(unsigned char*)GetParPtr(ptr->max,0) )
                    {
                        // max value is depend on another Par.
                        // max is the par no.
                        result = 1;
                        *(unsigned char*)dst = *(unsigned char*)GetParPtr(ptr->max,0);
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(unsigned char*)dst > ptr->max )
                    {
                        *(unsigned char*)dst = ptr->max;
                        result = 2;
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(unsigned char*)dst < *(unsigned char*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        *(unsigned char*)dst = *(unsigned char*)GetParPtr(ptr->min,0);
                        result = 3;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(unsigned char*)dst < ptr->min )
                    {
                        *(unsigned char*)dst = ptr->min;
                        result = 4;
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((unsigned char*)dst)++;
                ((unsigned char*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPFLOAT :
            /* EasyCODE ( 0 
            float parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(float*)dst > *(float*)GetParPtr(ptr->max,0) )
                    {
                        /* EasyCODE < */
                        // max value is depend on another Par.
                        // max is the par no.
                        /* EasyCODE > */
                        *(float*)dst = *(float*)GetParPtr(ptr->max,0);
                        result = 5;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(float*)dst > ptr->max )
                    {
                        *(float*)dst = ptr->max;
                        result = 6;
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(float*)dst < *(float*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        *(float*)dst = *(float*)GetParPtr(ptr->min,0);
                        result = 7;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(float*)dst < ptr->min )
                    {
                        *(float*)dst = ptr->min;
                        result = 8;
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((float*)dst)++;
                ((float*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPDBL :
            /* EasyCODE ( 0 
            double type parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(double*)dst > *(double*)GetParPtr(ptr->max,0) )
                    {
                        /* EasyCODE < */
                        // max value is depend on another Par.
                        // max is the par no.
                        /* EasyCODE > */
                        *(double*)dst = *(double*)GetParPtr(ptr->max,0);
                        result = 9;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(double*)dst > ptr->max )
                    {
                        *(double*)dst = ptr->max;
                        result = 10;
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(double*)dst <  *(double*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        *(double*)dst =  *(double*)GetParPtr(ptr->min,0);
                        result = 11;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(double*)dst < ptr->min )
                    {
                        *(double*)dst = ptr->min;
                        result = 12;
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((double*)dst)++;
                ((double*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPUINT :
            /* EasyCODE = */
        case TYPINT :
            /* EasyCODE ( 0 
            int und uint type parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(int*)dst > *(int*)GetParPtr(ptr->max,0) )
                    {
                        /* EasyCODE < */
                        // max value is depend on another Par.
                        // max is the par no.
                        /* EasyCODE > */
                        *(int*)dst = *(int*)GetParPtr(ptr->max,0);
                        result = 13;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(int*)dst > ptr->max )
                    {
                        *(int*)dst = ptr->max;
                        result = 14;
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(int*)dst < *(int*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        *(int*)dst = *(int*)GetParPtr(ptr->min,0);
                        result = 15;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(int*)dst < ptr->min )
                    {
                        *(int*)dst = ptr->min;
                        result = 16;
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((int*)dst)++;
                ((int*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPLONG :
            /* EasyCODE ( 0 
            long type parameter */
            for ( j = 0; j < ptr->size; j++ )
            {
                if ( ptr->ref & MAXDEP )
                {
                    /* EasyCODE ( 0 
                    max value is dependent */
                    if ( *(long*)dst > *(long*)GetParPtr(ptr->max,0) )
                    {
                        /* EasyCODE < */
                        // max value is depend on another Par.
                        // max is the par no.
                        /* EasyCODE > */
                        *(long*)dst = *(long*)GetParPtr(ptr->max,0);
                        result = 13;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    max value is defined */
                    if ( *(long*)dst > ptr->max )
                    {
                        *(long*)dst = ptr->max;
                        result = 14;
                        break;
                    }
                    /* EasyCODE ) */
                }
                if ( ptr->ref & MINDEP )
                {
                    /* EasyCODE ( 0 
                    min value is dependent */
                    if ( *(long*)dst < *(long*)GetParPtr(ptr->min,0) )
                    {
                        /* EasyCODE < */
                        // min value is depend on another Par.
                        // min is the par no.
                        /* EasyCODE > */
                        *(long*)dst = *(long*)GetParPtr(ptr->min,0);
                        result = 15;
                        break;
                    }
                    /* EasyCODE ) */
                }
                else
                {
                    /* EasyCODE ( 0 
                    min value is defined */
                    if ( *(long*)dst < ptr->min )
                    {
                        *(long*)dst = ptr->min;
                        result = 16;
                        break;
                    }
                    /* EasyCODE ) */
                }
                ((long*)dst)++;
                ((long*)src)++;
            }
            /* EasyCODE ) */
            break;
        case TYPUNDEF :
            break;
        }
        ptr++;
    }
    return result;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
LoadDefault(ptr) */
/* EasyCODE F */
void KlassePar::LoadDefault(st_parameter* ptr)
{
    /* EasyCODE < */
    // load only one given parameter to default values
    // if the parameter is an array, all members will be loaded
    /* EasyCODE > */
    /* EasyCODE < */
    const void* src;
    void* dst;
    int j;
    dst = ptr->pVar; // address of parameter
    src = ptr->def; // address of default values
    /* EasyCODE > */
    switch ( ptr->typ )
    {
    case TYPCH :
        /* EasyCODE = */
    case TYPBOOL :
        /* EasyCODE = */
    case TYPONOFF :
        /* EasyCODE = */
    case TYPUPDN :
        /* EasyCODE = */
    case TYPUCH :
        /* EasyCODE ( 0 */
        for ( j = 0; j < ptr->size; j++ )
        {
            if ( ptr->ref & DEFDEP )
            {
                // default value is the parameter no. of max
                // permitted value for this parameter.
                *(char*)dst = (char)GetDefault(ptr,j);
            }
            else
            {
                *(char*)dst = *(char*)src;
            }
            ((char*)dst)++;
            ((char*)src)++;
        }
        /* EasyCODE ) */
        break;
    case TYPFLOAT :
        /* EasyCODE ( 0 */
        for ( j = 0; j < ptr->size; j++ )
        {
            if ( ptr->ref & DEFDEP )
            {
                /* EasyCODE < */
                // default value is the parameter no. of max
                // permitted value for this parameter.
                /* EasyCODE > */
                *(float*)dst = (float)GetDefault(ptr,j);
            }
            else
            {
                *(float*)dst = *(float*)src;
            }
            ((float*)dst)++;
            ((float*)src)++;
        }
        /* EasyCODE ) */
        break;
    case TYPDBL :
        /* EasyCODE ( 0 */
        for ( j = 0; j < ptr->size; j++ )
        {
            if ( ptr->ref & DEFDEP )
            {
                /* EasyCODE < */
                // default value is the parameter no. of max
                // permitted value for this parameter.
                /* EasyCODE > */
                *(double*)dst = (double)GetDefault(ptr,j);
            }
            else
            {
                *(double*)dst = *(double*)src;
            }
            ((double*)dst)++;
            ((double*)src)++;
        }
        /* EasyCODE ) */
        break;
    case TYPUINT :
        /* EasyCODE = */
    case TYPINT :
        /* EasyCODE ( 0 */
        for ( j = 0; j < ptr->size; j++ )
        {
            if ( ptr->ref & DEFDEP )
            {
                /* EasyCODE < */
                // default value is the parameter no. of max
                // permitted value for this parameter.
                /* EasyCODE > */
                *(int*)dst = (int)GetDefault(ptr,j);
            }
            else
            {
                *(int*)dst = *(int*)src;
            }
            ((int*)dst)++;
            ((int*)src)++;
        }
        /* EasyCODE ) */
        break;
    case TYPLONG :
        /* EasyCODE ( 0 */
        for ( j = 0; j < ptr->size; j++ )
        {
            if ( ptr->ref & DEFDEP )
            {
                /* EasyCODE < */
                // default value is the parameter no. of max
                // permitted value for this parameter.
                /* EasyCODE > */
                *(long*)dst = (long)GetDefault(ptr,j);
            }
            else
            {
                *(long*)dst = *(long*)src;
            }
            ((long*)dst)++;
            ((long*)src)++;
        }
        /* EasyCODE ) */
        break;
    case TYPUNDEF :
        break;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Operate(taste) */
/* EasyCODE F */
unsigned char KlassePar::Operate(unsigned char taste)
{
    // process the given command (hier as pressed key)
    int tempvar;
    unsigned char result = FALSE;
    switch ( taste )
    {
    case TGRPNRINKR :
        /* EasyCODE ( 0 
        Group + */
        // H group Up
        tempvar = GetNextGroup(group);
        if ( tempvar != -1 )
        {
            group = tempvar;
            tempvar = GetMinIndexInGroup(group);
            if ( tempvar )
            {
                index = (char)tempvar;
                arr = 0;
                result = TRUE;
            }
        }
        else
        {
            /* EasyCODE < */
            // no change
            /* EasyCODE > */
        }
        /* EasyCODE ) */
        break;
    case TGRPNRDEKR :
        /* EasyCODE ( 0 
        Group - */
        // H group Down
        tempvar = GetPrevGroup(group);
        if ( tempvar != -1 )
        {
            group = tempvar;
            tempvar = GetMaxIndexInGroup(group);
            if ( tempvar )
            {
                index = (char)tempvar;
                arr = 0;
                result = TRUE;
            }
        }
        else
        {
            /* EasyCODE < */
            // no change
            /* EasyCODE > */
        }
        /* EasyCODE ) */
        break;
    case TPARNRINKR :
        /* EasyCODE ( 0 
        index + */
        // Par.No. increment
        tempvar = GetNextIndex(group,index);
        if ( tempvar > 0 )
        {
            index = tempvar;
            arr = 0;
            result = TRUE;
        }
        /* EasyCODE ) */
        break;
    case TPARNRDEKR :
        /* EasyCODE ( 0 
        index - */
        // Par.No. decrement
        tempvar = GetPrevIndex(group,index);
        if ( tempvar > 0 )
        {
            index = tempvar;
            arr = 0;
            result = TRUE;
        }
        /* EasyCODE ) */
        break;
    case TPARARRINKR :
        /* EasyCODE ( 0 
        arr + */
        /* EasyCODE < */
        // Array index increment
        /* EasyCODE > */
        if ( GetStructPointer(group*100+index)->size > arr+1 )
        {
            arr++;
            result = TRUE;
        }
        else
        {
            tempvar = GetStructPointer(group*100+index)->size - 1 ;
            if ( tempvar == arr )
            {
                result = TRUE;
            }
        }
        /* EasyCODE ) */
        break;
    case TPARARRDEKR :
        /* EasyCODE ( 0 
        arr - */
        /* EasyCODE < */
        // Array index decrement
        /* EasyCODE > */
        if ( arr > 0 )
        {
            arr--;
            result = TRUE;
        }
        else
        {
            if ( arr )
            {
                arr = 0;
                result = TRUE;
            }
        }
        /* EasyCODE ) */
        break;
    case TPARVALINKR :
        /* EasyCODE ( 0 
        Par increment */
        if ( (group == 9) &&
             ((index==2) || (index==3)) )
        {
        }
        else
        {
            // increment parameter
            IncPar(group*100+index,arr,.01);
            Norm();
        }
        result = TRUE;
        /* EasyCODE ) */
        break;
    case TPARVALDEKR :
        /* EasyCODE ( 0 
        Par decrement */
        if ( (group == 9) &&
             ((index==2) || (index==3)) )
        {
        }
        else
        {
            // decrement parameter
            DecPar(group*100+index,arr,.01);
            Norm();
        }
        result = TRUE;
        /* EasyCODE ) */
        break;
    default:
        result = TRUE;
    }
    if ( key == 1 )
    {
        return FALSE;
    }
    else
    {
        return result;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
Invalidate() */
/* EasyCODE F */
char KlassePar::Invalidate(char refresh)
{
    // this function updates the parameter no. & value on main display
    // Main display would not be repaint !
    // returns FALSE if current parameter(group:index)is not permitted to display
    int pn;
    st_parameter* ptr;
    char valstr[20];
    char arrstr[20];
    char str[50];
    if ( refresh )
    {
        pn = group*100+index;
        ptr = GetStructPointer(pn);
        if ( IsFreeToShow(ptr) )
        {
            if ( prtName )
            {
                sprintf(str,ESCS"=008060 %-40s",ptr->pName);
                eing_botsch1.senden(str);
            }
            sprintf(str,ESCS"=240083%-9s",ptr->pUnit);// unitstr
            eing_botsch1.senden(str);
            sprintf(str,FONT2 ESCS"=051109%-3d",pn);//parnum
            eing_botsch1.senden(str);
            if ( ptr->size > 1 )
            {
                sprintf(arrstr,"%2d:",arr+1);
            }
            else
            {
                sprintf(arrstr,"   ");
            }
            /* EasyCODE ( 0 
            sprint Parameter value */
            switch ( GetValStr(pn,valstr,arr) )
            {
            case 2 :
                sprintf(str,ESCS"=150109%s"ESCS"=206109         "ESCS"=214109    %-1s"FONT1,arrstr,valstr);
                break;
            case 3 :
                sprintf(str,ESCS"=150109%s"ESCS"=206109         "ESCS"=218109   %-2s"FONT1,arrstr,valstr);
                break;
            case 4 :
                sprintf(str,ESCS"=150109%s"ESCS"=206109         "ESCS"=224109  %-3s"FONT1,arrstr,valstr);
                break;
            case 5 :
                sprintf(str,ESCS"=150109%s"ESCS"=206109         "ESCS"=229109 %-4s"FONT1,arrstr,valstr);
                break;
            case 6 :
                sprintf(str,ESCS"=150109%s"ESCS"=206109         "ESCS"=233109%-5s"FONT1,arrstr,valstr);
                break;
            case 7 :
                sprintf(str,ESCS"=150109%s"ESCS"=206109         "ESCS"=225109%-6s"FONT1,arrstr,valstr);
                break;
            case 8 :
                sprintf(str,ESCS"=150109%s"ESCS"=206109         "ESCS"=220109%-7s"FONT1,arrstr,valstr);
                break;
            case 9 :
                sprintf(str,ESCS"=150109%s"ESCS"=206109         "ESCS"=216109%-8s"FONT1,arrstr,valstr);
                break;
            default:
                sprintf(str,ESCS"=150109%s"ESCS"=206109         "ESCS"=212109%-8s"FONT1,arrstr,valstr);
            }
            /* EasyCODE ) */
            eing_botsch1.senden(str);
            return TRUE;
        }
        else
        {
            /* EasyCODE < */
            // display of
            // parameter
            // is not permitted
            /* EasyCODE > */
            return FALSE;
        }
    }
    else
    {
        /* EasyCODE < */
        // future
        // expansion
        /* EasyCODE > */
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
ShowName() */
/* EasyCODE F */
char KlassePar::ShowName(char enDis)
{
    // this methode en/disables the show name
    // function on display
    char old = prtName;
    prtName = enDis;
    return old;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
FindNextVisible() */
/* EasyCODE < */
// ueberprueft ob jetzige group und index sichtbar sind.
// Falls nicht, findet der nexte.
// return TRUE if found
// die Variblen 'group & index' werden hier eventuell aktualisiert.
/* EasyCODE > */
/* EasyCODE F */
char KlassePar::FindNextVisible()
{
    char tempvar;
    unsigned char utemp = 0;
    while ( IsFreeToShow(GetStructPointer(group*100+index)) == FALSE )
    {
        if ( (tempvar = GetNextIndex(group,index)) ==  -1 )
        {
            /* EasyCODE < */
            //in this group there is nothing visible
            /* EasyCODE > */
            if ( (tempvar=GetNextGroup(group)) == -1 )
            {
                if ( utemp == 0 )
                {
                    // overrun
                    index = 1;
                    group = 1;
                    utemp = 1;
                }
                else
                {
                    /* EasyCODE < */
                    // nothing permitted
                    // to show
                    /* EasyCODE > */
                    return FALSE;
                }
            }
            else
            {
                // next group
                group = tempvar;
                index = 1;
            }
        }
        else
        {
            // next index
            // within group
            index = tempvar;
        }
    }
    return TRUE;
}

/* EasyCODE ) */
/* EasyCODE ( 0 
IsFreeToEdit(st_parameter* ptr) */
/* EasyCODE F */
char KlassePar::IsFreeToEdit(st_parameter* ptr)
{
    if ( ptr->freigabe <= freigabe )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
IsFreeToShow(st_parameter* ptr) */
/* EasyCODE F */
char KlassePar::IsFreeToShow(st_parameter* ptr)
{
    if ( (ptr->freigabe < ACC4) || (freigabe >= ACC4) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* EasyCODE ) */
/* EasyCODE ( 0 
CopmareWith */
/* EasyCODE F */
int KlassePar::CompareWith(KlassePar* const pPar)
{
    if ( size == pPar->size )
    {
        // in destructor will be compared
        pCompObj = pPar;
        return 1;
    }
    else
    {
        /* EasyCODE < */
        // not comparable
        /* EasyCODE > */
        return 0;
    }
}

/* EasyCODE ) */
/* EasyCODE ) */
/* EasyCODE ) */
