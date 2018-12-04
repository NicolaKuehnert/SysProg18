#ifndef display_H
#define display_H
#ifdef __cplusplus
extern "C" {
#endif
#define percent(x) ((100 / x->size) * count_elements(x))
/** Prototypen */
void display(void);
#ifdef __cplusplus
}
#endif

#endif
