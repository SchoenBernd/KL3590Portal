/* EasyCODE V8 */
/* EasyCODE ( 0 
hp3590.h,   Header fuer das Hauptprogramm,
            Ersteller: Wilfried Halfmann   EES 2856 */
// Task-Anzahl
#define ANZTASK 11

// Task-Indizes
#define KoorIndex     0
#define ZeitIndex     1
#define AblIndex      2
#define IoIndex       3
#define ScreenIndex   4
#define KeybIndex     5
#define NmInfIndex    6
#define EingIndex     7
#define NisIndex      8
#define Ref2Index     9
#define SchablIndex  10
//#define SMInfIndex   11

// Task-Prioritaeten
#define MAIN_PRIOR   1
#define KOOR_PRIOR   5
#define ZEIT_PRIOR   5
#define ABL_PRIOR    3
#define IO_PRIOR     3
#define SCREEN_PRIOR 3
#define KEYB_PRIOR   3
#define NMINF_PRIOR  3
#define EING_PRIOR   2
#define TIMER_PRIOR  6
#define NIS_PRIOR    4
#define REF2_PRIOR   7
#define SCHABL_PRIOR 3
//#define SMINF_PRIOR  3
/* EasyCODE C */
/* EasyCODE < */
// Struktur der Taskdaten
typedef struct
/* EasyCODE > */
{
        unsigned char wert;
        unsigned char intervall;
        unsigned char prior;
        Semaphore s;
        void (* handler)(void);
}taskdata;
/* EasyCODE E */
/* EasyCODE ( 0 
KlasseTask */
/* EasyCODE C */
/* EasyCODE < */
class KlasseTask
/* EasyCODE > */
{
        private:
public:
        taskdata td;
        TaskHandle handle;
        /* EasyCODE F */
        KlasseTask (taskdata tdini)
        {
            td = tdini;
            td.s = RTKCreateSemaphore(Binary,0,"Zeitsemaphore");
        }

        /* EasyCODE F */
        void inst (void)
        {
            handle = RTKCreateTask (td.handler, td.prior, 1024, "Task");
        }

        /* EasyCODE F */
        void dekr (void)
        {
            if ( --td.wert==0 )
            {
                RTKSignal(td.s);
                td.wert=td.intervall;
            }
        }

};
/* EasyCODE E */
/* EasyCODE ) */
/* EasyCODE < */
extern Mailbox anzbox;
extern struct safdat_struct *saf;
extern struct prodstruct *prod;
extern unsigned char *datdir;   // Directory fr Naehdaten
extern unsigned int dprseg;
extern unsigned int safseg;
extern int emsseg;
extern struct text *tz;
extern taskdata tdini [];
/* EasyCODE > */
/* EasyCODE ) */
