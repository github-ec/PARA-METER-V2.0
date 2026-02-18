import pyttsx3, time

# Stand 2026/02/18
# Ggf. anzupassende Parameter!
##############################
WordListFile = r"header.h"
mp3Path      = r'C:\temp\mp3\/'
##############################

engine = pyttsx3.init()
engine.setProperty('voice', '\x05de')

def get_word_arrays_from_file(filepath):
    """
    Liest WordList.h ein, entfernt ", ; und Zeilenbrüche,
    splittet bei / und gibt eine Liste von Listen (Arrays) zurück.
    """
    all_categories = []
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            for line in f:
                # Whitespace und Zeilenumbruch entfernen
                clean = line.strip()
                
                if "PROGMEM" in clean:
                    p = clean.find('"')
                    clean = clean[p:]
                else:
                    continue
                    
                # Semikolon am Ende entfernen (falls vorhanden)
                if clean.endswith(';'):
                    clean = clean[:-1].strip()
                
                # Anführungszeichen am Anfang und Ende entfernen
                if clean.startswith('"') and clean.endswith('"'):
                    clean = clean[1:-1]
                
                # Nur verarbeiten, wenn die Zeile nach dem Säubern nicht leer ist
                if clean:
                    # Splitten und jedes Wort zusätzlich trimmen (Sicherheit)
                    words = [w.strip() for w in clean.split('/') if w.strip()]
                    all_categories.append(words)
                    
        return all_categories

    except FileNotFoundError:
        print(f"Fehler: {filepath} nicht gefunden.")
        return []



wort_sammlung = get_word_arrays_from_file(WordListFile)
count = 0
for kategorie in wort_sammlung:
    print(f"Kategorie geladen mit {len(kategorie)} Begriffen.")
    count += len(kategorie)
print()    
print("Insgesamt",count,"Begriffe")

zeit = time.time()

kat = 0;
for kategorie in wort_sammlung:
    print(f"Kategorie geladen mit {len(kategorie)} Begriffen.")
    count = 0
    for entry in kategorie:
        Name = "%02i" % kat
        Name += "%02i" % count
        fname = mp3Path+Name+entry+".mp3"
        print(fname)
        count += 1
        engine.save_to_file(entry, fname)
    kat += 1   
print(time.time()-zeit)
engine.runAndWait()
