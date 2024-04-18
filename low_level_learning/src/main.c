#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "parse.h"
#include "file.h"

// TODO: Make remove employee by name -> '-r <name>'?
// TODO: Make a function to update employees hours

void print_usage(char *argv[]){
    printf("Usage: %s-n -f <database file>\n", argv[0]);
    printf("\t -n - create new database file \n");
    printf("\t - f - (required) path to database file\n");
    return;
}

int main(int argc, char *argv[]) {
    char *filepath      = NULL;
    char *portarg       = NULL;
    char *addstring     = NULL;
    char *remove        = NUUL;
    unsigned short port = 0;
    bool newfile        = false;
    bool list           = false;
    int c;

    int dbfd = -1;
    struct dbheader_t *dbhdr = NULL;
    struct employee_t *employees = NULL;

    while ((c = getopt(argc, argv, "nf:a:l")) != -1) {
        switch (c) {
            case 'n':
                newfile = true;
                break;
            case 'a':
                addstring = optarg;
                break;
            case 'f':
                filepath = optarg;
                break;
            case 'p':
                portarg = optarg;
                break;
            case 'l':
                list = true;
                break;
            case 'r':
                remove = optarg;
                break;
            case '?':
                printf("Unknown option -%c\n", c);
                break;
            default:
                return -1;
        }
    }
    if (filepath == NULL) {
        printf("Filepath is a required argument\n");
        print_usage(argv);

        return 0;
    }

    if (newfile) {
        dbfd = create_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("Unable to create database file \n");
            return -1;
        }
        if (create_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
            printf("Failed to create database header\n");
            return -1;
        }
    } else {
        dbfd = open_db_file(filepath);
        if(dbfd == STATUS_ERROR){
            printf("Unable to open database file\n");
            return -1;
        }
        if(validate_db_header(dbfd, &dbhdr) == STATUS_ERROR) {
            printf("Failed to validate database header\n");
            return -1;
        }
    }

    if(read_employees(dbfd, dbhdr, &employees) != STATUS_SUCCESS){
        printf("Failed to read employees\n");
        return 0;
    }
    if (addstring){
        dbhdr->count++;
        employees = realloc(employees, dbhdr->count*(sizeof(struct employee_t)));
        add_employee(dbhdr, employees, addstring);
    }
    if (list){
        list_employees(dbhdr, employees); 
    }
    output_file(dbfd, dbhdr, employees);

    printf("Newfile: %d\n", newfile);
    printf("Filepath: %s\n", filepath);

    return 0;
}
