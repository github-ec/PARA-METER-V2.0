import csv
import time

# Stand 2026/02/18
# ec2021

##############################
# Ggf. anzupassende Parameter
##############################
csv_input = "worte.csv"
csv_Delimiter  = ";"
header_output = "header.h"
##############################


def read_csv_columns_as_strings(file_path, delimiter=';'):
    """
    Liest eine CSV-Datei ein und fügt die Inhalte jeder Spalte 
    zu einem mit Schrägstrichen getrennten String zusammen.
    """
    column_data = []

    with open(file_path, mode='r', encoding='utf-8') as csv_file:
        reader = csv.reader(csv_file, delimiter=delimiter)
        
        for row_index, row in enumerate(reader):
            if row_index == 0:
                column_data = [cell for cell in row]
            else:
                for col_index, cell in enumerate(row):
                    if col_index < len(column_data) and cell.strip():
                        column_data[col_index] += '/' + cell
                        
    return column_data

def write_arduino_header(column_strings, output_filename):
    """
    Schreibt die extrahierten Spalten-Strings in eine C++ Header-Datei
    mit tabellarischer Übersicht im Kommentarbereich.
    """
    tm = time.localtime()
    time_string = "// Created {:04d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}\n".format(
        tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec
    )
    
    with open(output_filename, mode='w', encoding='utf-8') as header_file:
        header_file.write("#pragma once\n\n")
        header_file.write(time_string)
        
        # Tabellen-Header im Kommentar
        header_file.write("// ------------------------------------------\n")
        header_file.write("// ID  | Category Name    | Words\n")
        header_file.write("// ----|------------------|----------\n")
        
        # Zeilenweise die Metadaten der Kategorien schreiben
        for i, content in enumerate(column_strings):
            word_count = content.count('/') + 1 if content else 0
            # Extrahiert den Namen (erster Teil vor dem ersten /)
            category_name = content.split('/')[0]
            
            # Formatierung:
            # ID: 2 Stellen rechts
            # Name: 16 Stellen links
            # Words: 6 Stellen rechts
            line = f"// {i:>2}  | {category_name:<16} | {word_count:>6}\n"
            header_file.write(line)
            
        header_file.write("// ------------------------------------------\n\n")
        
        # 1. Einzelne String-Definitionen (PROGMEM)
        for i, content in enumerate(column_strings):
            header_file.write(f'const char p{i}[] PROGMEM = "{content}";\n')
        
        header_file.write("\n")
        
        # 2. Das Pointer-Array (Table)
        pointer_names = [f"p{i}" for i in range(len(column_strings))]
        table_content = ", ".join(pointer_names)
        header_file.write(f"const char* const cat_table[] PROGMEM = {{{table_content}}};\n\n")
        
        # 3. Konstante für die Anzahl
        header_file.write("constexpr int NumberOfCategories = sizeof(cat_table) / sizeof(cat_table[0]);\n")

if __name__ == "__main__":

    
    try:
        extracted_data = read_csv_columns_as_strings(csv_input, delimiter=csv_Delimiter)
        write_arduino_header(extracted_data, header_output)
        print(f"Erfolg: '{header_output}' wurde generiert.")
    except FileNotFoundError:
        print(f"Fehler: Die Datei '{csv_input}' wurde nicht gefunden.")


