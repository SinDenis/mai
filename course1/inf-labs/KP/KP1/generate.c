#include "generate.h"

int main()
{
    FILE *students = fopen("Students.txt", "w");
    db_table table_students = db_table_create("Students");

    db_table_add_field(&table_students, "Surname", CHAR, 30);
    db_table_add_field(&table_students, "Initials", CHAR, 4);
    db_table_add_field(&table_students, "Sex", CHAR, 1);
    db_table_add_field(&table_students, "Group", CHAR, 20);
    db_table_add_field(&table_students, "Matan", INTEGER, 1);
    db_table_add_field(&table_students, "Algebra", INTEGER, 1);
    db_table_add_field(&table_students, "DM", INTEGER, 1);
    db_table_add_field(&table_students, "Informatic", INTEGER, 1);

    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Akbulatova", "O.", "W", "M80-107B-18", 5, 3, 5, 4);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Andreev", "I.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Vsiliev", "M.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Gamov", "P.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Drachev", "M.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Egorova", "A.", "W", "M80-107B-18", 5, 4, 4, 5);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Ilminskiy", "N.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Kamesh", "M.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Karykh", "M.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Kobzev", "L.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Kronberg", "A.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Kupcov", "I.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Ladoga", "D.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Lysikov", "T.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Onichenko", "A.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Pavlukevich", "V.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Petrin", "S.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Poley-Dobronravova", "A.", "W", "M80-107B-18", 5, 4, 5, 4);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Sazonov", "N.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Sin", "D.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Suslov", "V.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Toyakov", "A.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Tokarev", "N.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Khrenov", "G.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Capkov", "A.", "M", "M80-107B-18", 5, 4, 5, 3);
    db_table_add_entry(&table_students, "%s%s%s%s%d%d%d%d", "Shichko", "A.", "M", "M80-107B-18", 5, 4, 5, 3);

    db_table_save(&table_students);

    db_table_print_txt(students, &table_students);

    return 0;
}
