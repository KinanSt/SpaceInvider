#include "../include/list.h"

#include <stdio.h>
#include <stdlib.h>


void ajouter_debut(struct ListDoubleChaine* list, void* newValue) {

  // Creating new element
  struct DoubleChaine* newElement = malloc(sizeof(struct DoubleChaine));
  newElement->next = list->first;
  newElement->previous = NULL;
  newElement->value = newValue;

  if (list->first != NULL) {
    // Updating first element
    list->first->previous = newElement;
  } else {
    list->last = newElement;
  }

  // Update list
  list->first = newElement;
}

void ajouter_fin(struct ListDoubleChaine* list, void* newValue) {

  // Creating new element
  struct DoubleChaine* newElement = malloc(sizeof(struct DoubleChaine));
  newElement->previous = list->last;
  newElement->next = NULL;
  newElement->value = newValue;

  if(list->last != NULL) {
    // Updating last element
    list->last->next = newElement;
  } else {
    list->first = newElement;
  }

  // Update list
  list->last = newElement;
}

void ajouter_apres(struct ListDoubleChaine* list, void* newValue, struct DoubleChaine* previousElement) {

  // Creating new element
  struct DoubleChaine* newElement = malloc(sizeof(struct DoubleChaine));
  newElement->previous = previousElement;
  newElement->next = previousElement->next;
  newElement->value = newValue;

  // Updating previous element
  if (previousElement->next != NULL)
    previousElement->next->previous = newElement;
  previousElement->next = newElement;

  if (previousElement == list->last) {
    // Update list
    list->last = newElement;
  }
}


void supprimer_debut(struct ListDoubleChaine* list) {

  struct DoubleChaine* secondElement = list->first->next;

  secondElement->previous = NULL;

  free(list->first);

  // if is now empty
  if (list->first == list->last) list->last = NULL;

  list->first = secondElement;
}

void supprimer_fin(struct ListDoubleChaine* list) {

  struct DoubleChaine* previousElement = list->last->previous;

  previousElement->next = NULL;

  free(list->last);

  // if is now empty
  if (list->first == list->last) list->first = NULL;

  list->last = previousElement;
}

void supprimer_apres(struct ListDoubleChaine* list, struct DoubleChaine* previousElement) {

  struct DoubleChaine* elementToDelete = previousElement->next;
  
  if (elementToDelete == NULL) return;


  
  previousElement->next = elementToDelete->next;

  if (elementToDelete->next == NULL) list->last = previousElement;
  else elementToDelete->next->previous = previousElement;

  free(elementToDelete);
}

void supprimer(struct ListDoubleChaine* list, struct DoubleChaine* element) {

  if (element == NULL) return;

  if (element->previous != NULL) {
    element->previous->next = element->next;
  } else {
    list->first = element->next;
  }

  if (element->next != NULL) {
    element->next->previous = element->previous;
  } else {
    list->last = element->previous;
  }

  free(element);
}
