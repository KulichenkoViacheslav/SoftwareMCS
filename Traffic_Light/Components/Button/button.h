#ifndef BUTTON_H
#define BUTTON_H

typedef void (*button_pressed_cb)(void);

void button_init(button_pressed_cb p_button_pressed_cb);
void button_check(void);

#endif