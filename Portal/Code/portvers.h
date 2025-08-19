/* EasyCODE V8 */
/* EasyCODE ( 0 
portvers.h */
#ifdef NF120x70
    // Naehfeld 1200x700
    // max 8 Zeichen !!!!!
    #define KENNUNG "Port/017"          // KENNUNG fuer Bedienfeld, Versionsnr. muss mit der
    
    
    #define MASCHINENKLASSE "3590-5X/12070"
    #define RFID_MASCHINENKLASSE "3590-12070"  // Maschinenklassen-String fÅr RFID darf nur
                                               // 10 Zeichen lang sein. Kennung fÅr Oberteil wird weg gelassen.
#endif 
#ifdef NF120x170
    // Naehfeld 1200x1700
    #define KENNUNG "Port/010"            // KENNUNG fuer Bedienfeld, Versionsnr. muss mit der
                                          // der Software-Versionsnr. uebereinstimmen
    
    #define MASCHINENKLASSE "3590-5X/120170"
    #define RFID_MASCHINENKLASSE "3590120170"  // Maschinenklassen-String fÅr RFID darf nur
                                               // 10 Zeichen lang sein. Kennung fÅr Oberteil wird weg gelassen.
#endif 
#ifdef NF100x50
#endif 
#ifdef NF120x200
    // Naehfeld 1200x2000
    
    #define KENNUNG "Port/003"          // KENNUNG fuer Bedienfeld, Versionsnr. muss mit der
                                        // der Software-Versionsnr. uebereinstimmen
    
    #define MASCHINENKLASSE "3590-5X/120200"
    #define RFID_MASCHINENKLASSE "3590120200"  // Maschinenklassen-String fÅr RFID darf nur
                                               // 10 Zeichen lang sein. Kennung fÅr Oberteil wird weg gelassen.
#endif //
extern unsigned char *softversion;
extern unsigned char *bs3version;
extern unsigned char *bs3version_schwer;
/* EasyCODE ) */
