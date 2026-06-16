
#ifndef LIST_H
#define LIST_H


typedef struct DoubleChaine {
  struct DoubleChaine* previous;
  struct DoubleChaine* next;

  void* value;
} DoubleChaine;

typedef struct ListDoubleChaine {
  struct DoubleChaine* first;
  struct DoubleChaine* last;
} ListDoubleChaine;


void ajouter_debut(ListDoubleChaine* list, void* newValue);

void ajouter_fin(ListDoubleChaine* list, void* newValue);

void ajouter_apres(ListDoubleChaine* list, void* newValue, DoubleChaine* previousElement);

void supprimer_debut(ListDoubleChaine* list);

void supprimer_fin(ListDoubleChaine* list);

void supprimer_apres(ListDoubleChaine* list, DoubleChaine* previousElement);

void supprimer(ListDoubleChaine* list, DoubleChaine* element);

#endif