#ifndef YIPIT_HEADLESS_H
#define YIPIT_HEADLESS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct y_hollow y_hollow_t;

y_hollow_t *y_hollow_new (void);
void y_hollow_free (y_hollow_t *hollow);
char *y_hollow_load (y_hollow_t *hollow, const char *url);

#ifdef __cplusplus
}
#endif

#endif
