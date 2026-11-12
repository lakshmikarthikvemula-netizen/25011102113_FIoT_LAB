#ifndef BME_LAB_H
#define BME_LAB_H

#include <stddef.h>

int bme_lab_start(void);
int bme_lab_json(char *body, size_t capacity);

#endif
