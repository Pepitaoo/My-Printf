#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
// pourcentage: %c, %s, %p, %d, %o, %u, %x


typedef struct s_graouuh { //creer une structure pour creer des pointeurs sur fonctions

    char flags;
    int (*pt)(va_list);

}t_graouuh;

int my_strlen(char* word) { //fonction qui permet de savoir la taille de string
    int index = 0;
    while(word[index] != '\0') {
        index++;
    }
    return index;
}

int intlen(int number) { //fonction pour la taille des numéros
    int buffer = number;
    int index = 0;

    while(buffer) {
        buffer /= 10;
        index += 1;
    }
    return index;
}

// %c
int characters(va_list args) { 
    int letter = va_arg(args, int); //letter = arg, puis l'écrire
    write(1,&letter, 1);

    return 1;
}

// %s
int char_star(va_list args ) {
    char* word = va_arg(args, char*); //word = arg

    if (word == NULL) // pour traiter NULL
    {
        write(1, "(null)", 6);
        return 6;
    }

    int size = my_strlen(word); // on regarde sa taille
    
    write(1,word, size); //puis on écrit le mot
     
    return size;
}

// %p
int void_star(va_list args ) {
    unsigned long number = (unsigned long)va_arg(args, void *);
    char* basemin = "0123456789abcdef"; // la bibliothèque de void
    int index = 0;
    char* buffer = malloc(sizeof(int) * intlen(number));
    int second_index;

    write(1,"0x", 2);
    while(number) {
        buffer[index] = basemin[number % 16];
        number /= 16; // sert a trouver l'adresse avec la basemin
        index++;
    }

    second_index = index - 1; // récupère l'arg

    while(second_index >= 0) {
        write(1, &buffer[second_index], 1); //écrit le résultat
        second_index--;
    }

    free(buffer);
    return index +2;

}

// %u
int posipi_number(va_list args ) {
    int number = va_arg(args, int);
    int index = 0;
    char* buffer = malloc(sizeof(char) * intlen(number));
    int second_index;

    while(number) {
        buffer[index] = '0' + (number % 10);
        number /= 10; // stock les chiffres
        index++;
    }
    second_index = index - 1; //récupère l'arg

    while(second_index >= 0) {
        write(1, &buffer[second_index], 1);
        second_index--; //écrit le résultat
    }
    free(buffer);
    return index;
}

// %d
int number(va_list args ) {
    int number;
    char* buffer;
    int index;
    int second_index;
    int sign = 0;

    number = va_arg(args, int); // récupère l'arg %d

    if(number == 0) { //gère le cas 0
        write(1, "0", 1);
        return (1);
    }

    if (number < 0){ // gère le signe des nombres négatifs
        write(1, "-", 1);
        sign = 1;
        number = -number;
    }

    buffer = malloc(sizeof(char) * intlen(number)); // allocation de mémoir
    index = 0; // pour le remplissage

    while(number) {
        buffer[index] = '0' + (number % 10); // stock les chiffres 1 par 1
        number /= 10;
        index++;
    }
    second_index = index - 1; // se placer à la fin

    while (second_index >= 0) //on met dans le bonne ordre et on l'écrit
    {
        write(1, &buffer[second_index], 1);
        second_index--;
    }

    free(buffer);

    return index + sign; //return les chiffres

}


// %o
int octal_number(va_list args ) {
    int number = va_arg(args, int);
    int index = 0;
    char* buffer = malloc(sizeof(int) * intlen(number));
    int second_index;

    while(number) {
        buffer[index] = '0' + (number % 8);
        number /= 8; // stock les chiffres
        index++;
    }
    second_index = index - 1; //récupère l'arg

    while(second_index >= 0) {
        write(1, &buffer[second_index], 1); //écrit le résultat
        second_index--;
    }
    free(buffer);
    return index;

}

// %x
int hexa_number(va_list args ) {
    int number = va_arg(args, int);
    char* basemaj = "0123456789ABCDEF"; //la bibliothèque hexa
    int index = 0;
    char* buffer = malloc(sizeof(int) * intlen(number));
    int second_index;

    while(number) {
        buffer[index] = basemaj[number % 16];
        number /= 16; // stock les chiffres
        index++;
    }
    second_index = index - 1; //récupère l'arg

    while(second_index >= 0) {
        write(1, &buffer[second_index], 1);
        second_index--; //écrit le résultat
    }

    free(buffer);
    return index;

}

int my_printf(char* string, ...) {
    va_list list;
    va_start(list, string);
    
    t_graouuh graouh[] = { //rempli la structure 
        {'d', number},
        {'c', characters},
        {'s', char_star},
        {'p', void_star},
        {'u', posipi_number},
        {'o', octal_number},
        {'x', hexa_number},
        {0, 0}
    };
    
    int inside_string;
    int index;
    int size = 0;


    inside_string = 0;

    while (string[inside_string] != '\0')
    {
        if (string[inside_string] == '%') //on trouve le %
        {
            inside_string++;

            index = 0;

            while (graouh[index].flags != 0)
            {
                if (string[inside_string] == graouh[index].flags) //on retrouve la bonne lettre
                {
                    size += graouh[index].pt(list); //on va dans la fonction correspondante et on prend la size
                    break;
                }

                index++;
            }
        }
        else
        {
            write(1, &string[inside_string], 1); //si le % correspond a rien
            size++;
        }

        inside_string++;
    
    }   
    va_end(list);   
    return size;
}


// int main(){
//     int selene = 42;
//     my_printf("char : %c\n", 'A');
//     my_printf("char_star : %s\n", "graouuh");
//     my_printf("number : %d\n", selene);
//     my_printf("posipi : %u\n", selene);
//     my_printf("octal : %o\n", selene);
//     my_printf("hexa : %x\n", selene);
//     my_printf("void_star : %p\n", &selene);
//     return (0);
// }

