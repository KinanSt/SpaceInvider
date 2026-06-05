
#ifndef LIST_H
#define LIST_H


struct DoubleChaine {
  struct DoubleChaine* previous;
  struct DoubleChaine* next;

  void* value;
};

struct ListDoubleChaine {
  struct DoubleChaine* first;
  struct DoubleChaine* last;
};


void ajouter_debut(struct ListDoubleChaine* list, void* newValue);

void ajouter_fin(struct ListDoubleChaine* list, void* newValue);

void ajouter_apres(struct ListDoubleChaine* list, void* newValue, struct DoubleChaine* previousElement);

void supprimer_debut(struct ListDoubleChaine* list);

void supprimer_fin(struct ListDoubleChaine* list);

void supprimer_apres(struct ListDoubleChaine* list, struct DoubleChaine* previousElement);

void supprimer(struct ListDoubleChaine* list, struct DoubleChaine* element);

#endif