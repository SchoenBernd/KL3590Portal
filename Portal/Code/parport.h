/* EasyCODE V8 */
/* EasyCODE ( 0 
par3590.h */
/* EasyCODE ( 0 
#defines */
/* EasyCODE ( 0 
Parameter-Typ */
/* EasyCODE < */
// Parameter type definitions
#define TYPCH       1
#define TYPINT      2
#define TYPFLOAT    3
#define TYPDBL      4
#define TYPBOOL     5
#define TYPONOFF    6
#define TYPUPDN     7
#define TYPUCH      8
#define TYPUINT     9
#define TYPLONG     10
#define TYPUNDEF    255
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE < */
//#define MAXGRP    99 // maximale Parameter-Gruppe-Nummer
#define NOMATCH     -1
/* EasyCODE > */
/* EasyCODE ( 0 
Freigabe Eigenschaften */
/* EasyCODE < */
#define ACC0        0   // Bits 0-3 define user access level.
#define ACC1        1   // Alle Parameter mit Freigabestuffe ACC0 sind immer sichtbar
#define ACC2        2
#define ACC3        3
#define ACC4        4
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE ( 0 
Abhaengigkeit Eigenschaften */
/* EasyCODE < */
#define NODEP       0x00    // not dependent
// Default value is a par. number.To get the right value you should read that. Default value must be an integer.
#define DEFDEP      0x20
// another parameter defines the min value. min value in structure is its par.index number(same group)
#define MINDEP      0x40
// another parameter defines the max value. max value in structure is its par.index number(same group)
#define MAXDEP      0x80
/* EasyCODE > */
/* EasyCODE ) */
/* EasyCODE [ 
Adresse von saf */
/* EasyCODE < */
#define SAF  ((struct safdat_struct *)MK_FP(safseg,0))
/* EasyCODE > */
/* EasyCODE ] */
/* EasyCODE ) */
/* EasyCODE ( 0 
Parameter structure */
/* EasyCODE C */
/* EasyCODE < */
typedef  struct
/* EasyCODE > */
{
        /* EasyCODE < */
        const char*  pName; // Zeiger auf Namen des Parameters
        int   faktor;        // Parameter = par*parFaktor
        int   group;         // die hunderte Gruppe 1 ,2 ,3 ....
        int   index;        // Parameternummer  01,02,...,99
        long   min;            // maximal Wert
        long   max;            // minimal Wert
        int   size;            // 1 bedeutet kein array
        const void* def;    // Zeiger auf default Wert
        int   typ;            // char , int , bool ...
        char  freigabe;        // Ein Parameterobjekt mit >=freigabe hat Zugriff auf Parameter
        char  ref;            // ob dieser Parameter abhaengig von anderen ist.
        void* pVar;            // Zeiger auf parameter
        const char*  pUnit; // Zeiger auf Einheiterklaerung string des Parameters
        char  Invalid;    // TRUE wenn geaendert
        /* EasyCODE > */
}st_parameter;
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ( 0 
class KlassePar */
/* EasyCODE C */
/* EasyCODE < */
class KlassePar
/* EasyCODE > */
{
        private:
        KlassePar* pCompObj;
public:
        /* EasyCODE ( 0 
        Data members */
        unsigned char key;        // main key
        char index;               // current parameter index number 1..99
        int group;                // current group No.
        int arr;                  // current index of parameter array
        char freigabe;            // Access level of object to parameters.Bigger value has more access.
        st_parameter* pSt;        // pointer to parameter structures first member.it is not permitted to change this after first init.
        int size;                 // Number of parameters
        unsigned char lastKey;    // help variable to keyboard handle
        char prtName;             // if TRUE then the parameter name will be displayed
        unsigned char parameterinit_enable;
        /* EasyCODE ) */
private:
        /* EasyCODE ( 0 
        Private Funktionen */
        st_parameter* GetStructPointer(unsigned int number);
        char GetValStr(unsigned int number,char* pStr,unsigned char index=0);
        char GetNextGroup(char istGrp);
        char GetPrevGroup(char istGrp);
        char GetNextIndex(char istGroup,char istIndex);
        char GetPrevIndex(char istGroup,char istIndex);
        char GetMaxIndexInGroup(char istGroup);
        char GetMinIndexInGroup(char istGroup);
        unsigned char Operate(unsigned char taste);
        char Invalidate(char refresh);
        double GetUpperLimit(int number);
        double GetLowerLimit(int number);
        char FindNextVisible();
        char IsFreeToEdit(st_parameter* ptr);
        char IsFreeToShow(st_parameter* ptr);
        char GetPar(void* ptr,int type,double* dest,int n=0);
        /* EasyCODE ) */
public:
        /* EasyCODE ( 0 
        Public Funktionen */
        KlassePar(void);
        ~KlassePar(void);
        KlassePar(const KlassePar& par);
        KlassePar(const KlassePar* par);
        const KlassePar& operator =(const KlassePar& par);
        KlassePar(st_parameter* pStruct,int size);
        void* GetParPtr(unsigned int number,unsigned char n=0);
        unsigned char grundzustand(void);
        char GetParType(unsigned int number);
        int GetParIndex(unsigned int number);
        int GetSize();
        char SetFreigabe(char level);
        char GetFreigabe(void);
        int GetArraySize(unsigned int number);
        void SetPar(unsigned int number,void* ptr,unsigned char n = 0);
        void IncPar(unsigned int number,unsigned char n=0,double grew=1);
        void DecPar(unsigned int number,unsigned char n=0,double grew=1);
        char KaltStart(void);
        char CheckUp(char corr);
        void LoadDefault(st_parameter* ptr);
        void Init(st_parameter* ptr,int s);
        char ShowName(char endis);
        double GetDefault(int number,int n);
        double GetDefault(st_parameter* ptr,int n=0);
        char GetParVal(unsigned int number,double* dest,unsigned char n=0);
        char GetParDef(unsigned int number,double* dest,unsigned char n=0);
        char GetParDef(st_parameter* ptr,double* dest,unsigned char n=0);
        char Norm();
        char GetUnitStr(unsigned int number,char* pStr);
        int  CompareWith(KlassePar* const pPar);
        char IsChanged(unsigned int number); // Ob ein Parameter geaendert ist
        void DelChangedInfo (unsigned int number);
        void parameterinit(void);
        /* EasyCODE ) */
};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE ) */
